
#ifndef PROXY_SESSION_CONTEXT_H
#define PROXY_SESSION_CONTEXT_H

#include "Uncopyable.h"

namespace proxy
{
    
class Session;

class SessionContext : private Uncopyable
{
public:

    enum class Type : uint8_t
    {
        SERVER_FD,
        SERVER_CONN_FD,
        CLIENT_CONN_FD
    };

    SessionContext(Type type_, Session& session_) : type(type_), session(&session_)
    {}

    const Type type;
    Session* const session;

private:

    SessionContext() = delete;
};

  
}

#endif
