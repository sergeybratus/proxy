#include "proxy/Proxy.h"

#include <memory>

namespace proxy
{
  
Proxy::Proxy(const std::vector<SessionConfig>& config) : epoll_fid(-1)
{
    for(auto& item : config)
    {      
      this->sessions.push_back(
	std::unique_ptr<Session>(new Session(item))
      );
    }
}

}

