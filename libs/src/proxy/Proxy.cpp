#include "proxy/Proxy.h"

#include "easylogging++.h"

namespace proxy
{
  
Proxy::Proxy(const ProxyConfig& config_) : config(config_)
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

      ProcessConnection(server_fd);
  }

  return false;
}

void Proxy::ProcessConnection(FileDesc& server_fd)
{
    std::error_code ec;
    FileDesc client_fd(Connect(ec));

    if(ec)
    {
        LOG(WARNING) << "Error connecting: " << ec.message();
        return;
    }

    // now we have open fd's for both client and server we
    // enter an event loop waiting for data to read from either source

}

FileDesc Proxy::Connect(std::error_code& ec)
{
    FileDesc client_fd(socket(AF_INET, SOCK_STREAM, 0));
    if(!client_fd.IsValid())
    {
        ec = std::error_code(errno, std::system_category());
        return FileDesc();
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_addr.s_addr = config.client.address.s_addr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(config.client.port );

    char buffer[INET_ADDRSTRLEN];
    auto address = inet_ntop(AF_INET, &serveraddr.sin_addr, buffer, INET_ADDRSTRLEN);
    LOG(INFO) << "Initiating connection to " << address << ":" << config.client.port;

    auto res = connect(client_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    if(res < 0)
    {
        ec = std::error_code(errno, std::system_category());
        return FileDesc();
    }

    return client_fd;
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
  LOG(INFO) << "Listening on: " << address;

  return server_listen_fd;
}

}

