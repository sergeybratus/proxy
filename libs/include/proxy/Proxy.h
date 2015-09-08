
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <arpa/inet.h>

#include <cstdint>
#include <vector>
#include <memory>

#include "Session.h"
#include "Config.h"

namespace proxy
{
  
class Proxy
{
  
public:
  
  Proxy(const std::vector<SessionConfig>& config);
  
  void Run();
  
private:
  
  Proxy()  = delete;
  Proxy(const Proxy&) = delete;

  // epoll instance fid
  int epoll_fid;
    
  std::vector<std::unique_ptr<Session>> sessions;
  
};
  
}

#endif
