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

bool ServerListenEventHandler::Process(uint32_t events, IEPollContext& context, std::error_code& ec) {
    // These events are always raised even if you didn't register for them
    if (events & EPOLLHUP || events & EPOLLERR) {
        ec = Error::SERVER_LISTEN_ERROR;
        close(session->server_listen_fd);
        return false;
    }

    // This is the only other event we should see here
    if (!(events & EPOLLIN)) {
        ec = Error::UNKNOWN_SESSION_EVENT;
        close(session->server_listen_fd);
        return false;
    }

    // new connections just bounce the existing connections
    this->session->Reset();

    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    auto conn_fd = accept(session->server_listen_fd, (sockaddr *) &clientaddr, &clientlen);

    if (conn_fd < 1) {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    this->session->server_conn_fd = conn_fd;

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &clientaddr.sin_addr, buffer, INET_ADDRSTRLEN);

    std::cout << "Accepted connection from: " << address << std::endl;

    if (!SetNonBlocking(conn_fd, ec)) {
        return false;
    }

    return BeginClientConnection(context, ec);
}

bool ServerListenEventHandler::BeginClientConnection(IEPollContext& context, std::error_code& ec)
{
    session->client_conn_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if(session->client_conn_fd < 0)
    {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_addr.s_addr = session->config.client.address.s_addr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons( session->config.client.port );

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &serveraddr.sin_addr, buffer, INET_ADDRSTRLEN);

    std::cout << "Initiating connection to " << address << ":" << session->config.client.port << std::endl;

    auto res = connect(session->client_conn_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(res < 0 && (errno != EINPROGRESS))
    {
        session->Reset();

        // this isn't an error that should stop the event loop, just reset the session
        return true;
    }

    // monitor the client_fd for write-ability. We don't want to read any data from the other side until this is open and ready
    return context.Modify(EPOLL_CTL_ADD, session->client_conn_fd, EPOLLOUT, handlers->clientConnEventHandler, ec);
}

}


