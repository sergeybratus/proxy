
#ifndef PROXY_SESSION_INFO_H
#define PROXY_SESSION_INFO_H

#include "proxy/Session.h"
#include "proxy/ServerEventHandler.h"

#include <system_error>

namespace proxy
{
    
class SessionInfo : private Uncopyable
{
  
public:
  
  SessionInfo(const SessionConfig& config);

  bool Intialize(IEPollContext& epoll, std::error_code& ec);

private:

  bool BindAndListen(std::error_code& ec);

  Session session;
  ServerEventHandler serverListenerEventHandler;

  SessionInfo() = delete;
  
};
  
  
}

#endif
