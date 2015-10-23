
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <system_error>

#include "proxy/ProxyConfig.h"
#include "proxy/Uncopyable.h"
#include "proxy/FileDesc.h"
#include "plugin/ParserPlugin.h"

namespace proxy
{
  
class Proxy : private Uncopyable
{
  
public:
  
  Proxy(const ProxyConfig& config, IParserFactory& factory);
     
  bool Run(std::error_code& ec);    
  
private:

  FileDesc BindAndListen(std::error_code& ec);

  FileDesc AcceptConnection(const FileDesc& listen_fd, std::error_code& ec);

  
  Proxy() = delete;

  const ProxyConfig config;
  IParserFactory& factory;

};
  
}

#endif
