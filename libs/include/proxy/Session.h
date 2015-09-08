
#ifndef PROXY_SESSION_H
#define PROXY_SESSION_H

#include <cstdint>

#include "proxy/Config.h"

namespace proxy
{
    
class Endpoint
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
  Endpoint(const Endpoint&) = delete;
  
};
   
class Session
{
  
public:
  
  Session(const SessionConfig& config);
      
  Endpoint listening;
  Endpoint connecting;    
};
  
  
}

#endif
