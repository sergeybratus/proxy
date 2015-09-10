
#ifndef PROXY_SESSION_H
#define PROXY_SESSION_H

#include <cstdint>

#include "proxy/Config.h"
#include "proxy/Uncopyable.h"

namespace proxy
{
    
class Session : private Uncopyable
{
  
public:
  
  Session(const SessionConfig& config);

  // static configuration information
  const SessionConfig config;

  // fd of the server socket
  int server_listen_fd;

  // fd of the server connection socket
  int server_conn_fd;

  // fd of the client connection socket
  int client_conn_fd;

private:
    
  Session() = delete;
  
};
  
  
}

#endif
