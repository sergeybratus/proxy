
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <arpa/inet.h>

#include <cstdint>
#include <vector>
#include <memory>

#include "FDPair.h"
#include "Config.h"

namespace proxy
{
  
class Proxy
{
  
public:
  
  Proxy(const std::vector<SessionConfig>& config);   
  
  bool Init();
  
private:
  
  Proxy()  = delete;
  Proxy(const Proxy&) = delete;

  // epoll instance fid
  int epoll_fid;
    
  std::vector<std::unique_ptr<FDPair>> sessions;
  
};
  
}

#endif
