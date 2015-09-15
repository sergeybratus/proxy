#include "proxy/Proxy.h"

#include "EPollContext.h"

#include <cstring>
#include <iostream>

namespace proxy
{
  
Proxy::Proxy(const std::vector<SessionConfig>& config)
{
  for(auto& item : config)
  {
    this->sessions.push_back(std::unique_ptr<SessionInfo>(new SessionInfo(item)));
  }
}

bool Proxy::Run(std::error_code& ec)
{
  // TODO - 50 is arbitrary, make this configurable
  EPollContext epoll(static_cast<int>(3*sessions.size()+1), 50, ec);
   
  if(ec)
  {    
    return false;
  }

  for(auto& info : sessions)
  {
    if(!info->Intialize(epoll, ec))
    {
      return false;
    }
  }
  
  return epoll.Run(ec);    
}

}

