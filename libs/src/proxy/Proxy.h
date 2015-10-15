
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <system_error>

#include "proxy/Config.h"
#include "proxy/Uncopyable.h"
#include "proxy/FileDesc.h"

namespace proxy
{
  
class Proxy : private Uncopyable
{
  
public:
  
  Proxy(const Config& config);
     
  bool Run(std::error_code& ec);    
  
private:

  FileDesc BindAndListen(std::error_code& ec);

  FileDesc AcceptConnection(const FileDesc& listen_fd, std::error_code& ec);

  bool ForkSession(FileDesc& server_fd, std::error_code& ec);
  
  Proxy() = delete;

  const Config config;

};
  
}

#endif
