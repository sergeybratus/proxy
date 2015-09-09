
#ifndef PROXY_CONFIG_H
#define PROXY_CONFIG_H

#include <arpa/inet.h>
#include <cstdint>

namespace proxy
{
  
class EndpointConfig
{
  
public:
  
  EndpointConfig();
  EndpointConfig(in_addr address, uint16_t port);
     
  in_addr address;
  uint16_t port;
};
  
class SessionConfig
{    
  
public:
  
  EndpointConfig server;
  EndpointConfig client;
};
    
}

#endif
