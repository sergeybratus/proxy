#include "EPollContext.h"

namespace proxy
{

    EPollContext::EPollContext(int numFD, int maxEvents, std::error_code &ec) :
            MAX_EVENT(maxEvents),
            events(new epoll_event[maxEvents]),
            epoll_fd(epoll_create(numFD))
    {
        if (epoll_fd < 0)
        {
            ec = std::error_code(errno, std::system_category());
        }
    }

    EPollContext::~EPollContext()
    {
        delete[] events;
    }

    bool EPollContext::Run(std::error_code &ec)
    {
        while (!ec)
        {
            int n = epoll_wait(epoll_fd, events, MAX_EVENT, -1);

            if (n < 0)
            {
                ec = std::error_code(errno, std::system_category());
                return false;
            }

            for (int i = 0; i < n; ++i)
            {
                this->ProcessEvent(events[i], ec);
            }
        }

        return true;
    }

    bool EPollContext::ProcessEvent(epoll_event &event, std::error_code &ec)
    {
        auto handler = reinterpret_cast<IEventHandler*>(event.data.ptr);
        return handler->Process(event.events, *this, ec);
    }

    bool EPollContext::Modify(int operation, int fd, uint32_t events, IEventHandler& handler, std::error_code &ec)
    {
        epoll_event evt;
        evt.events = events;
        evt.data.ptr = &handler;

        if (epoll_ctl(epoll_fd, operation, fd, &evt) < 0)
        {
            ec = std::error_code(errno, std::system_category());
            return false;
        }

        return true;
    }


}


