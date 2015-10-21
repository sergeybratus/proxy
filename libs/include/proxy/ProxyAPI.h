
#ifndef PROXY_API_H
#define PROXY_API_H

#include "ProxyConfig.h"
#include "ParserPlugin.h"

#include <vector>
#include <system_error>

namespace proxy
{
  
bool Run(const ProxyConfig& config, IParserFactory& factory, std::error_code& ec);
    
}

#endif
