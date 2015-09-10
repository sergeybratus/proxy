
#ifndef PROXY_SOCKETHELPERS_H
#define PROXY_SOCKETHELPERS_H

#include <system_error>

namespace proxy
{
    bool SetNonBlocking(int fd, std::error_code &ec);
}

#endif //PROXY_SOCKETHELPERS_H
