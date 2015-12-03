
#ifndef PROXY_BUFFER_H
#define PROXY_BUFFER_H

#include <plugin/Uncopyable.h>

#include <plugin/RSlice.h>
#include <plugin/WSlice.h>

#include <memory>

namespace proxy {

    class Buffer final : private Uncopyable
    {

    public:

        Buffer(size_t size) :
                M_SIZE(size),
                m_buffer(std::unique_ptr<uint8_t[]>(new uint8_t[size]))
        {}

        RSlice ToRSlice() const
        {
            return RSlice(m_buffer.get(), M_SIZE);
        }

        WSlice GetWSlice()
        {
            return WSlice(m_buffer.get(), M_SIZE);
        }

        size_t Size() const { return M_SIZE; }

    private:
        Buffer() = delete;

        const size_t M_SIZE;
        std::unique_ptr<uint8_t[]> m_buffer;
    };

}

#endif //PROXY_BUFFER_H
