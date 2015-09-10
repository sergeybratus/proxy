#include "proxy/Proxy.h"

#include "EPollContext.h"

#include <cstring>
#include <iostream>

namespace proxy
{
  
Proxy::Proxy(const std::vector<SessionConfig>& config)
{
    this->InitSessions(config);
}

bool Proxy::Run(std::error_code& ec)
{
  EPollContext epoll(3*sessions.size()+1, 50, ec); // TODO - 50 is arbitrary
   
  if(ec)
  {    
    return false;
  }
  
  if(!this->BindAndListen())
  {
    return false;
  }   
  
  for(auto& info : sessions)
  {
    epoll.Modify(EPOLL_CTL_ADD, info->session.server_listen_fd, EPOLLIN, info->serverEventHandler, ec);
  }
  
  return epoll.Run(ec);    
}

bool Proxy::BindAndListen()
{
  for(auto& info : sessions)
  {
    info->session.server_listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if(info->session.server_listen_fd < 0)
    {
      std::cerr << "Error server fd: " << strerror(errno) << std::endl;
      return false;
    }
    
    sockaddr_in server_addr;        
   
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(info->session.config.server.port);
    server_addr.sin_addr.s_addr = htonl(info->session.config.server.address.s_addr);
    
    if(bind(info->session.server_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "bind() error server fd: " << strerror(errno) << std::endl;
        return false;
    }
    
    if(listen(info->session.server_listen_fd, 100) < 0)
    {
        std::cerr << "listen() error on server fd: " << strerror(errno) << std::endl;
        return false;
    }

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &server_addr.sin_addr, buffer, INET_ADDRSTRLEN);
    std::cout << "Bound to: " << address << std::endl;

  }  
  
  return true; 
  
}

void Proxy::InitSessions(const std::vector<SessionConfig>& config)
{
  for(auto& item : config)
  {
    this->sessions.push_back(std::unique_ptr<SessionInfo>(new SessionInfo(item)));
  }
}

}

