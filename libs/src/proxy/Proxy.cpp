#include "proxy/Proxy.h"

#include "easylogging++.h"
#include "ProxySession.h"

#include <proxy/ErrorCodes.h>

#include <sys/epoll.h>
#include <unistd.h>

namespace proxy
{
  
Proxy::Proxy(const ProxyConfig& config_, IParserFactory& factory_) : config(config_), factory(factory_)
{

}

bool Proxy::Run(std::error_code& ec)
{
  FileDesc listen_fd(BindAndListen(ec));

  while(!ec)
  {
      FileDesc server_fd(AcceptConnection(listen_fd, ec));

      // Here is where we will eventually spin up another thread or fork.
      // For the time being, just handle one connection in a blocking manner
      // until we've reached a PoC stage

      ProxySession session(config.client, server_fd, factory);

      session.Run();
  }

  return false;
}

FileDesc Proxy::AcceptConnection(const FileDesc& listen_fd, std::error_code& ec)
{
    sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    FileDesc conn_fd(accept(listen_fd, (sockaddr *) &clientaddr, &clientlen));

    if (conn_fd.IsValid())
    {
        char buffer[INET_ADDRSTRLEN];
        auto address = inet_ntop(AF_INET, &clientaddr.sin_addr, buffer, INET_ADDRSTRLEN);
        LOG(INFO) << "Accepted connection from: " << address << ":" << ntohs(clientaddr.sin_port);
    }
    else
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
  server_addr.sin_addr.s_addr = config.server.address.s_addr;

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

  char buffer[INET_ADDRSTRLEN];
  auto address = inet_ntop(AF_INET, &server_addr.sin_addr, buffer, INET_ADDRSTRLEN);
  LOG(INFO) << "Listening on: " << address << ":" << config.server.port;

  return server_listen_fd;
}

}

