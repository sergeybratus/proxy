
#include "proxy/Session.h"

namespace proxy
{
  
  Endpoint::Endpoint(const EndpointConfig& config_) : 
    config(config_),
    state(State::CLOSED),
    fid(-1)
  {}
  
  Session::Session(const SessionConfig& config) :
    listening(config.listening),
    connecting(config.connecting)
  {}
  
}
  
