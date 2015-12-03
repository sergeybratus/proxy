
#include "proxy/SocketHelpers.h"

#include <fcntl.h>

namespace proxy
{

    bool SetNonBlocking(int fd, std::error_code& ec)
    {
        const int FLAGS = fcntl(fd, F_GETFL, NULL);

        if(FLAGS == -1)
        {
            ec = std::error_code(errno, std::system_category());
            return false;
        }

        if(fcntl(fd, F_SETFL, FLAGS | O_NONBLOCK) == -1)
        {
            ec = std::error_code(errno, std::system_category());
            return false;
        }

        return true;
    }

}