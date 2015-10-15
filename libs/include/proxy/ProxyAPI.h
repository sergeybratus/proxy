
#ifndef PROXY_API_H
#define PROXY_API_H

#include "ProxyConfig.h"

#include <vector>
#include <system_error>

namespace proxy
{
  
bool Run(const ProxyConfig& config, std::error_code& ec);
    
}

#endif
