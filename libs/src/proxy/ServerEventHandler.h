
#ifndef PROXY_SERVER_EVENT_HANDLER_H
#define PROXY_SERVER_EVENT_HANDLER_H

#include "proxy/IEventHandler.h"
#include "proxy/Session.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
    
class ServerEventHandler final : public IEventHandler, private Uncopyable
{
  
public:

  ServerEventHandler(Session& session);

  virtual bool Process(uint32_t events, IEPollContext& context, std::error_code& ec) override;

private:

  ServerEventHandler() = delete;
    
  Session* const session;
  
};
  
  
}

#endif
