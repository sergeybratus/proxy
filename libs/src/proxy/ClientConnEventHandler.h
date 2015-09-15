
#ifndef PROXY_CLIENT_CONN_EVENT_HANDLER_H
#define PROXY_CLIENT_CONN_EVENT_HANDLER_H

#include "proxy/IEventHandler.h"
#include "proxy/Uncopyable.h"

namespace proxy
{

class Session;
class EventHandlers;
    
class ClientConnEventHandler final : public IEventHandler, private Uncopyable
{
  
public:

  ClientConnEventHandler(Session& session, EventHandlers& handlers);

  virtual bool Process(uint32_t events, IEPollContext& context, std::error_code& ec) override;

private:

  ClientConnEventHandler() = delete;
    
  Session* const session;
  EventHandlers* const handlers;
  
};
  
  
}

#endif
