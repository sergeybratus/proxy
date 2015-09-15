
#include "proxy/Session.h"

#include <unistd.h>

namespace proxy
{

  Session::Session(const SessionConfig& config_) :
          config(config_),
          server_listen_fd(-1),
          server_conn_fd(-1),
          client_conn_fd(-1)
  {}

  void Session::Reset()
  {
        if(this->server_conn_fd >= 0)
        {
            close(this->server_conn_fd);
            this->server_conn_fd = -1;
        }

        if(this->client_conn_fd >= 0)
        {
            close(this->client_conn_fd);
            this->client_conn_fd = -1;
        }
  }
  
}
  
