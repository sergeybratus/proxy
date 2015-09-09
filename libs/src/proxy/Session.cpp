
#include "proxy/Session.h"

namespace proxy
{
  
  Connection::Connection() :     
    state(State::CLOSED),
    fd(-1)
  {}
  
  Session::Session(const SessionConfig& config_) :
          serverfd(-1),
          config(config_),
          serverListenContext(SessionContext::Type::SERVER_FD, *this),
          serverConnContext(SessionContext::Type::SERVER_CONN_FD, *this),
          clientConnContext(SessionContext::Type::CLIENT_CONN_FD, *this)
  {}
  
}
  
