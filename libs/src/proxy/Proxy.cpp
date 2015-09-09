#include "proxy/Proxy.h"

#include "EPollContext.h"

#include <memory>
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
  
  for(auto& session : sessions)
  {
    epoll.AddListen(*session, ec);
  }
  
  return epoll.Run(ec);    
}

bool Proxy::BindAndListen()
{
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
        std::cerr << "bind() error server fd: " << strerror(errno) << std::endl;
        return false;
    }
    
    if(listen(session->serverfd, 100) < 0)
    {
        std::cerr << "listen() error on server fd: " << strerror(errno) << std::endl;
        return false;
    }
    
    std::cout << "Bound to: " << server_addr.sin_addr.s_addr << std::endl;
  }  
  
  return true; 
  
}

void Proxy::InitSessions(const std::vector<SessionConfig>& config)
{
  for(auto& item : config)
  {
    this->sessions.push_back(std::unique_ptr<Session>(new Session(item)));    
  }
}

}

