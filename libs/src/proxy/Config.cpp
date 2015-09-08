#include "proxy/Config.h"

namespace proxy
{
  
EndpointConfig::EndpointConfig(in_addr address_, uint16_t port_) : address(address_), port(port_)
{}
  
EndpointConfig::EndpointConfig() : address({.s_addr= 0}), port(20000)
{}  
  
}

