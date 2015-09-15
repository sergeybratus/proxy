
#ifndef PROXY_SERVER_LISTEN_EVENT_HANDLER_H
#define PROXY_SERVER_LISTEN_EVENT_HANDLER_H

#include "proxy/IEventHandler.h"
#include "proxy/Uncopyable.h"

namespace proxy
{

class Session;
class EventHandlers;

    
class ServerListenEventHandler final : public IEventHandler, private Uncopyable
{
  
public:

  ServerListenEventHandler(Session& session, EventHandlers& handlers);

  virtual bool Process(uint32_t events, IEPollContext& context, std::error_code& ec) override;

private:

  bool BeginClientConnection(IEPollContext& context, std::error_code& ec);

  ServerListenEventHandler() = delete;
    
  Session* const session;
  EventHandlers* const handlers;
  
};
  
  
}

#endif
