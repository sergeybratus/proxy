
#include "SessionInfo.h"

#include <string.h>
#include <iostream>
#include <sys/epoll.h>

namespace proxy
{

  SessionInfo::SessionInfo(const SessionConfig& config) :
    session(config),
    serverListenerEventHandler(session)
  {}

  bool SessionInfo::Intialize(IEPollContext& epoll, std::error_code& ec)
  {
    if(!BindAndListen(ec))
    {
        return false;
    }

    // register to be notified when there are incoming connection requests
    return epoll.Modify(EPOLL_CTL_ADD, session.server_listen_fd, EPOLLIN, serverListenerEventHandler, ec);
  }

  bool SessionInfo::BindAndListen(std::error_code& ec)
  {
      session.server_listen_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
      if(session.server_listen_fd < 0)
      {
          ec = std::error_code(errno, std::system_category());
          return false;
      }

      sockaddr_in server_addr;

      bzero(&server_addr, sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(session.config.server.port);
      server_addr.sin_addr.s_addr = htonl(session.config.server.address.s_addr);

      if(bind(session.server_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
      {
          ec = std::error_code(errno, std::system_category());
          return false;
      }

      if(listen(session.server_listen_fd, 100) < 0)
      {
          ec = std::error_code(errno, std::system_category());
          return false;
      }

      char buffer[INET_ADDRSTRLEN];
      auto address = inet_ntop(AF_INET, &server_addr.sin_addr, buffer, INET_ADDRSTRLEN);
      std::cout << "Bound to: " << address << std::endl;
      return true;
  }

}


