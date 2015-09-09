
#ifndef PROXY_SESSION_H
#define PROXY_SESSION_H

#include <cstdint>

#include "proxy/Config.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
    
class Connection : private Uncopyable
{

public:
  
  Connection();
  
  enum class State : uint8_t
  {
    CLOSED,
    OPEN  
  };            
  
  State state;
  int fd;
    
};
   
class Session : private Uncopyable
{
  
public:
  
  Session(const SessionConfig& config);
        
  // fid of the server socket
  int serverfd;
  
  SessionConfig config;
  
  Connection serverconn;
  Connection clientconn;
    
  Session() = delete;
  
};
  
  
}

#endif
