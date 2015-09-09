
#include "proxy/Session.h"

namespace proxy
{
  
  Connection::Connection() :     
    state(State::CLOSED),
    fd(-1)
  {}
  
  Session::Session(const SessionConfig& config_) : serverfd(-1), config(config_)
  {}
  
}
  
