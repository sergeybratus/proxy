
#include "proxy/ClientConnEventHandler.h"

namespace proxy
{

ClientConnEventHandler::ClientConnEventHandler(Session& session_, EventHandlers& handlers_) :
        session(&session_),
        handlers(&handlers_)
{}

bool ClientConnEventHandler::Process(uint32_t events, IEPollContext& context, std::error_code& ec)
{
    return false;
}

}


