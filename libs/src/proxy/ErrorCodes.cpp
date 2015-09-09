
#include "proxy/ErrorCodes.h"

namespace proxy
{

    ProxyErrorCategory ProxyErrorCategory::instance;

    std::string ProxyErrorCategory::message(int ev) const {
        switch (ev) {
            case (static_cast<int>(Error::SERVER_LISTEN_ERROR)):
                return "A previously listening server socket has encountered an error";
            default:
                return "Unknown error";
        }
    }

    std::error_code make_error_code(Error err)
    {
        return std::error_code(static_cast<int>(err), ProxyErrorCategory::Instance());
    }

}