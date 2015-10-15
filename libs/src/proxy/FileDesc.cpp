#include "FileDesc.h"

#include <unistd.h>
#include <utility>

namespace proxy
{
    FileDesc::FileDesc(int fd_) : m_fd(fd_)
    {}

    FileDesc::FileDesc(FileDesc&& other) : m_fd(other.m_fd)
    {
        other.m_fd = -1;
    }

    FileDesc::~FileDesc()
    {
        if(m_fd > 0)
        {
            close(m_fd);
        }
    }
}