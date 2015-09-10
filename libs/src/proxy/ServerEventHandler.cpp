#include "proxy/ServerEventHandler.h"

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

#include "proxy/ErrorCodes.h"
#include "proxy/SocketHelpers.h"

namespace proxy
{

ServerEventHandler::ServerEventHandler(Session& session_) :
        session(&session_)
{}

bool ServerEventHandler::Process(uint32_t events, IEPollContext& context, std::error_code& ec)
{
    // figure out what kind of event it is on the session
    if (events & EPOLLHUP || events & EPOLLERR)
    {
        ec = Error::SERVER_LISTEN_ERROR;
        close(session->server_listen_fd);
        return false;
    }

    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    int connfd = accept(session->server_listen_fd, (sockaddr *) &clientaddr, &clientlen);

    if (connfd < 1) {
        ec = std::error_code(errno, std::system_category());
        return false;
    }

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &clientaddr.sin_addr, buffer, INET_ADDRSTRLEN);

    std::cout << "Accepted connection from: " << address << std::endl;

    if(!SetNonBlocking(connfd, ec))
    {
        return false;
    }

    close(connfd);

    return true;
}

}


