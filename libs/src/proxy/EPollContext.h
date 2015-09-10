
#ifndef PROXY_EPOLL_CONTEXT_H
#define PROXY_EPOLL_CONTEXT_H

#include "proxy/Uncopyable.h"
#include "proxy/Session.h"
#include "proxy/IEPollContext.h"
#include "proxy/IEventHandler.h"

#include <system_error>
#include <sys/epoll.h>


namespace proxy
{
  
class EPollContext : public IEPollContext, private Uncopyable
{
  
public:
  
  EPollContext(int numFD, int maxEvents, std::error_code& ec);

  ~EPollContext();
  
  bool Run(std::error_code& ec);

  /// --- implement IEPollContext ---
  virtual bool Modify(int operation, int fd, uint32_t events, IEventHandler& handler, std::error_code& ec) override;
  
private:
  
  bool ProcessEvent(epoll_event& event, std::error_code& ec);
  
  EPollContext() = delete;
  
  const int MAX_EVENT;
  epoll_event* const events;
    
  int epoll_fd;
};
  
}

#endif
