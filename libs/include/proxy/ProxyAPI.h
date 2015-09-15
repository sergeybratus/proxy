
#ifndef PROXY_API_H
#define PROXY_API_H

#include "Config.h"

#include <vector>
#include <system_error>

namespace proxy
{
  
bool Run(const std::vector<SessionConfig>& config, std::error_code& ec);
    
}

#endif