
#ifndef PROXY_EPOLL_CONTEXT_H
#define PROXY_EPOLL_CONTEXT_H

#include "Uncopyable.h"

#include "Session.h"

#include <system_error>
#include <sys/epoll.h>


namespace proxy
{
  
class EPollContext : private Uncopyable
{
  
public:
  
  EPollContext(int numFD, int maxEvents, std::error_code& ec);

  ~EPollContext();
  
  bool Run(std::error_code& ec);

  bool AddListen(Session& session, std::error_code& ec);    
  
private:
  
  bool ProcessEvent(epoll_event& event, std::error_code& ec);

  bool ProcessServerListenEvent(const epoll_event& event, Session& session, std::error_code& ec);
  bool ProcessServerConnEvent(const epoll_event& event, Session& session, std::error_code& ec);
  bool ProcessClientConnEvent(const epoll_event& event, Session& session, std::error_code& ec);
  
  EPollContext() = delete;
  
  bool Modify(int operation, int fd, uint32_t events, SessionContext& context, std::error_code& ec);
  
  const int MAX_EVENT;
  epoll_event* const events;
    
  int epfd;
};
  
}

#endif
