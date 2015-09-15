#include "ServerListenEventHandler.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

#include "proxy/ErrorCodes.h"
#include "proxy/SocketHelpers.h"
#include "proxy/EventHandlers.h"

namespace proxy
{

ServerListenEventHandler::ServerListenEventHandler(Session& session_, EventHandlers& handlers_) :
        session(&session_),
        handlers(&handlers_)
{}

bool ServerListenEventHandler::Process(uint32_t events, IEPollContext& context, std::error_code& ec)
{
    // These events are always raised even if you didn't register for them
    if (events & EPOLLHUP || events & EPOLLERR)
    {
        ec = Error::SERVER_LISTEN_ERROR;
        close(session->server_listen_fd);
        return false;
    }

    // This is the only other event we should see here
    if(!(events & EPOLLIN))
    {
        ec = Error::UNKNOWN_SESSION_EVENT;
        close(session->server_listen_fd);
        return false;
    }

    // new connections just bounce the existing connections
    this->session->Reset();

    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    auto conn_fd = accept(session->server_listen_fd, (sockaddr *) &clientaddr, &clientlen);

    if (conn_fd < 1)
    {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    this->session->server_conn_fd = conn_fd;

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &clientaddr.sin_addr, buffer, INET_ADDRSTRLEN);

    std::cout << "Accepted connection from: " << address << std::endl;

    if(!SetNonBlocking(conn_fd, ec))
    {
        return false;
    }

    session->client_conn_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if(session->client_conn_fd < 0)
    {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    struct sockaddr_in dest;
    dest.sin_addr.s_addr = session->config.server.address.s_addr;
    dest.sin_family = AF_INET;
    dest.sin_port = htons( session->config.server.port );

    if((connect(session->client_conn_fd, (struct sockaddr *)&dest, sizeof(dest)) < 0))
    {
        // all errors except for an in-progress connection should just
        if(errno != EINPROGRESS)
        {
            session->Reset();
        }

        // either way, this isn't an error that should stop the event loop
        return true;
    }


    // monitor the client_fd for write-ability. We don't want to read any data until this is open.



    return true;
}

}


