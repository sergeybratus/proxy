
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <arpa/inet.h>

#include <cstdint>
#include <vector>
#include <memory>
#include <system_error>

#include "proxy/SessionInfo.h"
#include "proxy/Config.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
  
class Proxy : private Uncopyable
{
  
public:
  
  Proxy(const std::vector<SessionConfig>& config);
     
  bool Run(std::error_code& ec);    
  
private:
  
  Proxy() = delete;
      
  void InitSessions(const std::vector<SessionConfig>& config);
  
  bool BindAndListen();
    
  std::vector<std::unique_ptr<SessionInfo>> sessions;
  
};
  
}

#endif
