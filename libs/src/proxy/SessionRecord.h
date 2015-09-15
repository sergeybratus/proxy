
#ifndef PROXY_SESSION_RECORD_H
#define PROXY_SESSION_RECORD_H

#include "proxy/Session.h"
#include "proxy/ServerEventHandler.h"

#include <system_error>

namespace proxy
{
    
class SessionRecord : private Uncopyable
{
  
public:
  
  SessionRecord(const SessionConfig& config);

  bool Intialize(IEPollContext& epoll, std::error_code& ec);

private:

  bool BindAndListen(std::error_code& ec);

  Session session;
  ServerEventHandler serverListenerEventHandler;

  SessionRecord() = delete;
  
};
  
  
}

#endif
