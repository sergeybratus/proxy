
#ifndef PROXY_IEVENT_HANDLER_H
#define PROXY_IEVENT_HANDLER_H

#include "Session.h"
#include "IEPollContext.h"

#include <system_error>

namespace proxy
{

class IEventHandler
{
public:

    virtual bool Process(uint32_t events, IEPollContext& context, std::error_code& ec) = 0;

};

  
}

#endif
