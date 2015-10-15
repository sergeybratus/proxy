
#ifndef PROXY_CONFIG_H
#define PROXY_CONFIG_H

#include <arpa/inet.h>

#include <cstdint>
#include <cstring>
#include <system_error>

namespace proxy
{
  
class EndpointConfig
{
  
public:
  
  EndpointConfig();

  bool SetAddress(const std::string& address, std::error_code& ec);
     
  in_addr address;
  uint16_t port;
};
  
class Config
{    
  
public:
  
  EndpointConfig server;
  EndpointConfig client;
};
    
}

#endif
