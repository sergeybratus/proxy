
#ifndef PROXY_SESSION_RECORD_H
#define PROXY_SESSION_RECORD_H

#include "proxy/Session.h"
#include "EventHandlers.h"

#include <system_error>

namespace proxy
{
    
class SessionRecord : private Uncopyable
{
  
public:
  
  SessionRecord(const SessionConfig& config);

  bool Initialize(IEPollContext& epoll, std::error_code& ec);

private:

  bool BindAndListen(std::error_code& ec);

  Session session;
  EventHandlers handlers;

  SessionRecord() = delete;
  
};
  
  
}

#endif
