
#ifndef PROXY_BUFFER_H
#define PROXY_BUFFER_H

#include "proxy/Uncopyable.h"

#include "proxy/RSlice.h"
#include "proxy/WSlice.h"

#include <memory>

namespace proxy {

    class Buffer final : private Uncopyable
    {

    public:

        Buffer(size_t size) :
                M_SIZE(size),
                buffer(std::unique_ptr<uint8_t[]>(new uint8_t[size]))
        {}

        RSlice ToRSlice() const
        {
            return RSlice(buffer.get(), M_SIZE);
        }

        WSlice GetWSlice()
        {
            return WSlice(buffer.get(), M_SIZE);
        }

        size_t Size() const { return M_SIZE; }

    private:
        Buffer() = delete;

        const size_t M_SIZE;
        std::unique_ptr<uint8_t[]> buffer;
    };

}

#endif //PROXY_BUFFER_H
