
#include "proxy/ProxyAPI.h"

#include "Proxy.h"

namespace proxy
{
  
bool Run(const ProxyConfig& config, IParserFactory& factory, std::error_code& ec)
{
    Proxy proxy(config, factory);
    return proxy.Run(ec);
}
    
}


