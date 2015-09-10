
#ifndef PROXY_IEPOLL_CONTEXT_H
#define PROXY_IEPOLL_CONTEXT_H

#include <system_error>

namespace proxy
{
    class IEventHandler;

    class IEPollContext
    {

    public:

        virtual bool Modify(int operation, int fd, uint32_t events, IEventHandler &handler, std::error_code &ec) = 0;

    };

}

#endif
