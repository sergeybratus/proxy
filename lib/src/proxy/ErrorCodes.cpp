
#include "proxy/ErrorCodes.h"

#include <string>

namespace proxy
{

    ProxyErrorCategory ProxyErrorCategory::instance;

    std::string ProxyErrorCategory::message(int ev) const
    {
        switch (ev) {
            case (static_cast<int>(Error::SERVER_LISTEN_ERROR)):
                return "A previously listening server socket has encountered an error";
            case (static_cast<int>(Error::EPOLL_SOCKET_ERR)):
                return "epoll returned an error for an open socket";
            case (static_cast<int>(Error::INVALID_IPV4_ADDRESS)):
                return "Invalid IPv4 address";
            case(static_cast<int>(Error::END_OF_FILE)):
                return "read() returned 0";
            default:
                return "Unknown error";
        }
    }

    std::error_code make_error_code(Error err)
    {
        return std::error_code(static_cast<int>(err), ProxyErrorCategory::Instance());
    }

}