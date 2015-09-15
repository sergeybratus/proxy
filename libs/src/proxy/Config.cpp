#include "proxy/Config.h"

#include "proxy/ErrorCodes.h"

namespace proxy
{
  
bool EndpointConfig::SetAddress(const std::string& address_, std::error_code& ec)
{
    switch(inet_pton(AF_INET, address_.c_str(), &address.s_addr))
    {
        case(0):
            ec = Error::INVALID_IPV4_ADDRESS;
            return false;
        case(-1):
            ec = std::error_code(errno, std::system_category());
            return false;
        default:
            return true;
    }
}
  
EndpointConfig::EndpointConfig() : port(20000)
{
    address.s_addr = INADDR_LOOPBACK;
}
  
}

