
#ifndef PROXY_EPOLL_CONTEXT_H
#define PROXY_EPOLL_CONTEXT_H

#include "Uncopyable.h"

#include <system_error>

namespace proxy
{
  
class EPollContext : private Uncopyable
{
  
public:
  
  EPollContext(size_t size, std::error_code& ec);
    
  
private:
  
  EPollContext() = delete;
  
  int epfd;
};
  
}

#endif
