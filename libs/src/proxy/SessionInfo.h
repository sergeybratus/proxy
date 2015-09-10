
#ifndef PROXY_SESSION_INFO_H
#define PROXY_SESSION_INFO_H

#include "proxy/Session.h"
#include "proxy/ServerEventHandler.h"

namespace proxy
{
    
class SessionInfo : private Uncopyable
{
  
public:
  
  SessionInfo(const SessionConfig& config) :
    session(config),
    serverEventHandler(session)
  {}

  Session session;

  ServerEventHandler serverEventHandler;

private:

  SessionInfo() = delete;
  
};
  
  
}

#endif
