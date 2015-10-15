
#include "proxy/ProxyAPI.h"

#include "Proxy.h"

namespace proxy
{
  
bool Run(const ProxyConfig& config, std::error_code& ec)
{
    Proxy proxy(config);
    return proxy.Run(ec);
}
    
}


