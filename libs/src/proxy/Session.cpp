
#include "proxy/Session.h"

namespace proxy
{

  Session::Session(const SessionConfig& config_) :
          config(config_),
          server_listen_fd(-1),
          server_conn_fd(-1),
          client_conn_fd(-1)
  {}
  
}
  
