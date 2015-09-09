#include "proxy/Proxy.h"

#include <memory>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>

namespace proxy
{
  
bool Proxy::Run(const std::vector<SessionConfig>& config)
{
    Proxy proxy(config);
    return proxy.Run();    
}
  
Proxy::Proxy(const std::vector<SessionConfig>& config)
{
    this->InitSessions(config);
}

bool Proxy::Run()
{
  int epfd = epoll_create(3*sessions.size());
  
  if(epfd < 0)
  {
    std::cerr << "Error creating epoll fd: " << strerror(errno) << std::endl;
    return false;
  }
  
  for(auto& session : sessions)
  {
    session->serverfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(session->serverfd < 0)
    {
      std::cerr << "Error server fd: " << strerror(errno) << std::endl;
      return false;
    }
    
    sockaddr_in server_addr;
   
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(session->config.listening.port);
    server_addr.sin_addr.s_addr = htonl(session->config.listening.address.s_addr);
    
    if(bind(session->serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding server fd: " << strerror(errno) << std::endl;
        return false;
    }
  }
  
  return false;        
}

void Proxy::InitSessions(const std::vector<SessionConfig>& config)
{
  for(auto& item : config)
  {
    this->sessions.push_back(std::unique_ptr<Session>(new Session(item)));    
  }
}

}

