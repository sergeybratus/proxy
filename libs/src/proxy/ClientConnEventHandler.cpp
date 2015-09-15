
#include "proxy/ClientConnEventHandler.h"

#include <sys/epoll.h>

#include <proxy/ErrorCodes.h>
#include <iostream>

namespace proxy
{

ClientConnEventHandler::ClientConnEventHandler(Session& session_, EventHandlers& handlers_) :
        session(&session_),
        handlers(&handlers_)
{}

bool ClientConnEventHandler::Process(uint32_t events, IEPollContext& context, std::error_code& ec)
{
    // if the connection attempt fails or there's an error while connected
    if (events & EPOLLHUP || events & EPOLLERR)
    {
        std::cout << "Connection attempt failed!" << std::endl;
        this->session->Reset();
        return true;
    }

    if(events & EPOLLOUT) // ready for writing
    {
        std::cout << "Connected!" << std::endl;
    }

    this->session->Reset(); // TODO
    return true;
}

}


