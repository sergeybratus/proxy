#include "proxy/Proxy.h"

#include "easylogging++.h"

#include <sys/epoll.h>
#include <proxy/ErrorCodes.h>
#include <unistd.h>

namespace proxy
{
  
Proxy::Proxy(const ProxyConfig& config_) : config(config_)
{

}

bool Proxy::Run(std::error_code& ec)
{
  FileDesc listen_fd(BindAndListen(ec));

  while(!ec)
  {
      FileDesc server_fd(AcceptConnection(listen_fd, ec));

      // Here is where we will eventually spin up another thread or fork.
      // For the time being, just handle one connection in a blocking manner
      // until we've reached a PoC stage

      ProcessConnection(server_fd);
  }

  return false;
}

void Proxy::ProcessConnection(FileDesc& server_fd)
{
    std::error_code ec;
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
        LOG(ERROR) << "Error creating epoll fd: " << ec.message();
        return;
    }

    if(!RegisterForDataAvailable(epoll_fd, server_fd, ec) || !RegisterForDataAvailable(epoll_fd, client_fd, ec))
    {
        ec = std::error_code(errno, std::system_category());
        LOG(ERROR) << "Error registering epoll fd: " << ec.message();
        return;
    }

    while(!ec)
    {
        RunOne(epoll_fd, client_fd, server_fd, ec);
    }

}

bool Proxy::RunOne(FileDesc& epoll_fd, FileDesc& client_fd, FileDesc& server_fd, std::error_code &ec)
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
            return Transfer(client_fd, server_fd, ec);
        }
        else
        {
            // otherwise assume source is server id
            return Transfer(server_fd, client_fd, ec);
        }
    }
    else
    {
        ec = Error::EPOLL_SOCKET_ERR;
        return false;
    }
}

bool Proxy::Transfer(FileDesc& src, FileDesc& dest, std::error_code &ec)
{
    // until we're using an actual plugin, just read and write to a local buffer
    uint8_t buffer[4096];
    auto numRead = read(src, buffer, 4096);

    if(numRead < 0) {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    if(numRead == 0) {
        ec = Error::END_OF_FILE;
        return false;
    }

    auto numToWrite = numRead;
    uint8_t* pos = buffer;

    while(numToWrite > 0)
    {
        auto numWritten = write(dest, pos, numToWrite);

        if(numWritten <= 0)
        {
            ec = std::error_code(errno, std::system_category());
            return false;
        }

        numToWrite -= numWritten;
        pos += numWritten;
    }

    return true;
}

bool Proxy::RegisterForDataAvailable(const FileDesc& epoll_fd, const FileDesc& fd, std::error_code &ec)
{
    return Modify(epoll_fd, EPOLL_CTL_ADD, fd, EPOLLIN, ec);
}

bool Proxy::Modify(const FileDesc& epoll_fd, int operation, const FileDesc& fd, uint32_t events, std::error_code &ec)
{
    epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;

    if (epoll_ctl(epoll_fd, operation, fd, &evt) < 0)
    {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    return true;
}

FileDesc Proxy::Connect(std::error_code& ec)
{
    FileDesc client_fd(socket(AF_INET, SOCK_STREAM, 0));
    if(!client_fd.IsValid())
    {
        ec = std::error_code(errno, std::system_category());
        return FileDesc();
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_addr.s_addr = config.client.address.s_addr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(config.client.port );

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &serveraddr.sin_addr, buffer, INET_ADDRSTRLEN);
    LOG(INFO) << "Initiating connection to " << address << ":" << config.client.port;

    auto res = connect(client_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if(res < 0)
    {
        ec = std::error_code(errno, std::system_category());
        return FileDesc();
    }

    return client_fd;
}

FileDesc Proxy::AcceptConnection(const FileDesc& listen_fd, std::error_code& ec)
{
    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    FileDesc conn_fd(accept(listen_fd, (sockaddr *) &clientaddr, &clientlen));

    if (conn_fd.IsValid())
    {
        char buffer[INET_ADDRSTRLEN];
        auto address = inet_ntop(AF_INET, &clientaddr.sin_addr, buffer, INET_ADDRSTRLEN);
        LOG(INFO) << "Accepted connection from: " << address << ":" << ntohs(clientaddr.sin_port);
    }
    else
    {
        ec = std::error_code(errno, std::system_category());
    }

    return conn_fd;
}

FileDesc Proxy::BindAndListen(std::error_code& ec)
{
  FileDesc server_listen_fd(socket(AF_INET, SOCK_STREAM, 0));
  if(!server_listen_fd)
  {
    ec = std::error_code(errno, std::system_category());
    return server_listen_fd;
  }

  sockaddr_in server_addr;

  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(config.server.port);
  server_addr.sin_addr.s_addr = config.server.address.s_addr;

  if(bind(server_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
  {
    ec = std::error_code(errno, std::system_category());
    return FileDesc();
  }

  if(listen(server_listen_fd, 100))
  {
    ec = std::error_code(errno, std::system_category());
    return FileDesc();
  }

  char buffer[INET_ADDRSTRLEN];
  auto address = inet_ntop(AF_INET, &server_addr.sin_addr, buffer, INET_ADDRSTRLEN);
  LOG(INFO) << "Listening on: " << address;

  return server_listen_fd;
}

}

