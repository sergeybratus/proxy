
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <arpa/inet.h>

#include <cstdint>
#include <vector>
#include <memory>

#include "proxy/Session.h"
#include "proxy/Config.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
  
class Proxy : private Uncopyable
{
  
public:
     
  static bool Run(const std::vector<SessionConfig>& config);
  
private:
  
  Proxy() = delete;
  
  bool Run();
  
  Proxy(const std::vector<SessionConfig>& config);
  
  void InitSessions(const std::vector<SessionConfig>& config);  
    
  std::vector<std::unique_ptr<Session>> sessions;
  
};
  
}

#endif
