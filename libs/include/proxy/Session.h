
#ifndef PROXY_SESSION_H
#define PROXY_SESSION_H

#include <cstdint>

#include "proxy/Config.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
    
class Endpoint : private Uncopyable
{

public:
  
  enum class State : uint8_t
  {
    CLOSED,
    OPEN  
  };
  
  Endpoint(const EndpointConfig& config);
      
  const EndpointConfig config;
  
  State state;
  int fid;

private:
  Endpoint() = delete;
    
};
   
class Session : private Uncopyable
{
  
public:
  
  Session(const SessionConfig& config);
      
  Endpoint listening;
  Endpoint connecting; 
  
private:
  Session() = delete;
  
};
  
  
}

#endif
