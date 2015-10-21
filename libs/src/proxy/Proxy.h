
#ifndef PROXY_PROXY_H
#define PROXY_PROXY_H

#include <system_error>

#include "proxy/ProxyConfig.h"
#include "proxy/Uncopyable.h"
#include "proxy/FileDesc.h"
#include "proxy/ParserPlugin.h"

namespace proxy
{
  
class Proxy : private Uncopyable
{
  
public:
  
  Proxy(const ProxyConfig& config, const IParserFactory& factory);
     
  bool Run(std::error_code& ec);    
  
private:

  FileDesc BindAndListen(std::error_code& ec);

  FileDesc AcceptConnection(const FileDesc& listen_fd, std::error_code& ec);

  FileDesc Connect(std::error_code& ec);

  bool RegisterForDataAvailable(const FileDesc& epoll_fd, const FileDesc& fd, std::error_code &ec);
  bool Modify(const FileDesc& epoll_fd, int operation, const FileDesc& fd, uint32_t events, std::error_code &ec);

  void ProcessConnection(FileDesc& server_fd);

  bool RunOne(FileDesc& epoll_fd, FileDesc& client_fd, FileDesc& server_fd, std::error_code &ec);
  bool Transfer(FileDesc& src, FileDesc& dest, std::error_code &ec);

  
  Proxy() = delete;

  const ProxyConfig config;
  const IParserFactory& factory;

};
  
}

#endif
