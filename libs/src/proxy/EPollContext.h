
#ifndef PROXY_EPOLL_CONTEXT_H
#define PROXY_EPOLL_CONTEXT_H

#include "Uncopyable.h"

#include "Session.h"

#include <system_error>
#include <memory>
#include <sys/epoll.h>
#include <functional>

namespace proxy
{
  
class EPollContext : private Uncopyable
{
  
public:
  
  EPollContext(size_t numFD, int maxEvents, std::error_code& ec);   
  
  bool Run(std::error_code& ec);

  bool AddListen(Session& session, std::error_code& ec);    
  
private:
  
  bool ProcessEvent(epoll_event& event, std::error_code& ec);
  
  EPollContext() = delete;
  
  bool Modify(int operation, int fd, uint32_t events, Session& session, std::error_code& ec);
  
  const int MAX_EVENT;
  const std::unique_ptr<epoll_event[]> events;
    
  int epfd;
};
  
}

#endif
