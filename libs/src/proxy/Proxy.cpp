#include "proxy/Proxy.h"

namespace proxy
{
  
Proxy::Proxy(const ProxyConfig& config_) : config(config_)
{

}

bool Proxy::Run(std::error_code& ec)
{
  FileDesc listen_fd(BindAndListen(ec));

  if(ec)
  {
    return false;
  }

  while(!ec)
  {
      FileDesc server_fd(AcceptConnection(listen_fd, ec));

      // connection should be automatically closed, but here is where we'll fork
  }

  return false;
}

bool Proxy::ForkSession(FileDesc& listen_fd, std::error_code& ec)
{

    return false;
}

FileDesc Proxy::AcceptConnection(const FileDesc& listen_fd, std::error_code& ec)
{
    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    FileDesc conn_fd(accept(listen_fd, (sockaddr *) &clientaddr, &clientlen));

    if (!conn_fd.IsValid())
    {
        ec = std::error_code(errno, std::system_category());
    }

    return conn_fd;
}

FileDesc Proxy::BindAndListen(std::error_code& ec)
{
  FileDesc server_listen_fd(socket(AF_INET, SOCK_STREAM, 0));
  if(!server_listen_fd)
  {
    ec = std::error_code(errno, std::system_category());
    return server_listen_fd;
  }

  sockaddr_in server_addr;

  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(config.server.port);
  server_addr.sin_addr.s_addr = htonl(config.server.address.s_addr);

  if(bind(server_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
  {
    ec = std::error_code(errno, std::system_category());
    return FileDesc();
  }

  if(listen(server_listen_fd, 100))
  {
    ec = std::error_code(errno, std::system_category());
    return FileDesc();
  }

  /* TODO logging
  char buffer[INET_ADDRSTRLEN];
  auto address = inet_ntop(AF_INET, &server_addr.sin_addr, buffer, INET_ADDRSTRLEN);
  */

  return server_listen_fd;
}

}

