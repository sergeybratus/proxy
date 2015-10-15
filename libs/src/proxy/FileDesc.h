
#ifndef PROXY_FILEDESC_H
#define PROXY_FILEDESC_H

#include "Uncopyable.h"

namespace proxy {

    // An RAII class for file descriptors
    class FileDesc : private Uncopyable {

    public:
        explicit FileDesc(int fd = -1);

        FileDesc(FileDesc&& other);
        ~FileDesc();

        bool IsValid() const { return m_fd > 0; }

        operator int() const { return m_fd; }

    private:

        int m_fd;
    };


}



#endif //PROXY_FILEDESC_H
