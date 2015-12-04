
#include "ProxySession.h"

#include <proxy/ErrorCodes.h>

#include <easylogging++.h>

#include <sys/epoll.h>
#include <unistd.h>


namespace proxy
{

ProxySession::ProxySession(const ProxyConfig& config_, FileDesc& server_fd_, IParserFactory& factory) :
        m_config(config_),
        m_server_fd(std::move(server_fd_)),
        m_s2cParser(std::move(factory.Create(SessionDir::ServerToClient, *this))),
        m_c2sParser(std::move(factory.Create(SessionDir::ClientToServer, *this)))
{

}

void ProxySession::QueueWrite(const RSlice& output)
{
        this->m_output_vec.push_back(output);
}

void ProxySession::Run(std::error_code& ec)
{
        FileDesc client_fd(Connect(ec));

        if(ec)
        {
                LOG(WARNING) << "Error connecting: " << ec.message();
                return;
        }

        // now we have open fd's for both client and server we
        // enter an event loop waiting for data to read from either source

        FileDesc epoll_fd(epoll_create(2));

        if(!epoll_fd.IsValid())
        {
                ec = std::error_code(errno, std::system_category());
                return;
        }

        if(!RegisterForDataAvailable(epoll_fd, m_server_fd, ec) || !RegisterForDataAvailable(epoll_fd, client_fd, ec))
        {
                ec = std::error_code(errno, std::system_category());
                return;
        }

        while(!ec)
        {
                RunOne(epoll_fd, client_fd, ec);
        }
}

bool ProxySession::RunOne(FileDesc& epoll_fd, FileDesc& client_fd, std::error_code &ec)
{
    epoll_event event; // TODO - process more than 1 event at a time?

    int num = epoll_wait(epoll_fd, &event, 1, -1);

    if(num < 0)
    {
            ec = std::error_code(errno, std::system_category());
            return false;
    }

    if(event.events & EPOLLIN)
    {
            if(event.data.fd == client_fd)
            {
                    return Transfer(client_fd, m_server_fd, *m_c2sParser, ec);
            }
            else
            {
                    // otherwise assume source is server id
                    return Transfer(m_server_fd, client_fd, *m_s2cParser, ec);
            }
    }
    else
    {
            ec = Error::EPOLL_SOCKET_ERR;
            return false;
    }
}

bool ProxySession::Transfer(FileDesc& src, FileDesc& dest, IParser& parser, std::error_code &ec)
{
    // until we're using an actual plugin, just read and write to a local buffer
    auto inBuff = parser.GetWriteSlice();
    auto numRead = read(src, inBuff, inBuff.Size());

    if(numRead < 0) {
            ec = std::error_code(errno, std::system_category());
            return false;
    }

    if(numRead == 0) {
            ec = Error::END_OF_FILE;
            return false;
    }


    if(m_config.observeOnly)
    {
        // in observe only mode we just write the data we read immediately
        // there is no guarantee that the parser doesn't modify the buffer

        this->WriteAll(dest, RSlice(inBuff, numRead), ec);

        // regardless of if there is any error or not, we feed the parser
        // but ignore whatever it outputs
        parser.Parse(numRead);
    }
    else
    {
        // now notify the parser that we wrote some data into its input buffer
        if(parser.Parse(numRead))
        {
            // if the parser didn't reject it, pass output anything the parser kicked back
            this->WriteOutputTo(dest, ec);
        }
        else
        {
            ec = Error::PARSER_REJECT;
        }
    }

    // always clear any unwritten slices due to error or explicit parser rejection
    this->m_output_vec.clear();

    return ec.value();
}

bool ProxySession::WriteOutputTo(FileDesc& dest, std::error_code &ec)
{
    while(!ec && !this->m_output_vec.empty())
    {
        RSlice slice = m_output_vec.front();
        m_output_vec.pop_front();

        WriteAll(dest, slice, ec);
    }

    // in case of write failure, just clear the vec
    m_output_vec.clear();

    return ec.value();
}

bool ProxySession::WriteAll(FileDesc& dest, const RSlice& data, std::error_code &ec)
{
    RSlice remaining(data);

    while(!remaining.IsEmpty())
    {
        auto num = write(dest, remaining, remaining.Size());

        if (num <= 0) {
            ec = std::error_code(errno, std::system_category());
            return false;
        }

        remaining.Advance(num);
    }

    return true;
}


bool ProxySession::RegisterForDataAvailable(const FileDesc& epoll_fd, const FileDesc& fd, std::error_code &ec)
{
    return Modify(epoll_fd, EPOLL_CTL_ADD, fd, EPOLLIN, ec);
}

bool ProxySession::Modify(const FileDesc& epoll_fd, int operation, const FileDesc& fd, uint32_t events, std::error_code &ec)
{
    epoll_event evt = {
            .events = events,
            { .fd = fd }
    };

    if (epoll_ctl(epoll_fd, operation, fd, &evt) < 0)
    {
            ec = std::error_code(errno, std::system_category());
            return false;
    }

    return true;
}

FileDesc ProxySession::Connect(std::error_code& ec)
{
    FileDesc client_fd(socket(AF_INET, SOCK_STREAM, 0));
    if(!client_fd.IsValid())
    {
            ec = std::error_code(errno, std::system_category());
            return FileDesc();
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_addr.s_addr = m_config.client.address.s_addr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(m_config.client.port );

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &serveraddr.sin_addr, buffer, INET_ADDRSTRLEN);
    LOG(INFO) << "Initiating connection to " << address << ":" << m_config.client.port;

    auto res = connect(client_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if(res < 0)
    {
            ec = std::error_code(errno, std::system_category());
            return FileDesc();
    }

    LOG(INFO) << "Connected to " << address << ":" << m_config.client.port;

    return client_fd;
}

}
