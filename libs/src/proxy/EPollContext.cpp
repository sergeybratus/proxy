
#include "EPollContext.h"

#include <sys/epoll.h>

namespace proxy
{
  
EPollContext::EPollContext(size_t size, std::error_code& ec)
{
  this->epfd = epoll_create(size);
  if(epfd < 0)
  {
    ec = std::error_code(errno, std::system_category());
  }
}

}


