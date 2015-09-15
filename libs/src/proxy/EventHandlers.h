
#ifndef PROXY_EVENT_HANDLERS_H
#define PROXY_EVENT_HANDLERS_H

#include "proxy/Session.h"

#include "ServerListenEventHandler.h"
#include "ClientConnEventHandler.h"

#include <system_error>

namespace proxy
{
    
class EventHandlers : private Uncopyable
{
  
public:

    EventHandlers(Session& session) :
            serverListenerEventHandler(session, *this),
            clientConnEventHandler(session, *this)
    {}

    ServerListenEventHandler serverListenerEventHandler;
    ClientConnEventHandler clientConnEventHandler;

private:

    EventHandlers() = delete;
  
};
  
  
}

#endif
