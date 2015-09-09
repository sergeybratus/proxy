#include "proxy/Config.h"

namespace proxy
{
  
EndpointConfig::EndpointConfig(in_addr address_, uint16_t port_) : address(address_), port(port_)
{}
  
EndpointConfig::EndpointConfig() : port(20000)
{
    address.s_addr = INADDR_LOOPBACK;
}
  
}

