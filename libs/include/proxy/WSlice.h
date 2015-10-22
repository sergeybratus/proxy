
#ifndef PROXY_WSLICE_H
#define PROXY_WSLICE_H

#include <cstdint>

namespace proxy
{
    // A write-able "slice" of a buffer
    class WSlice
    {

        public:

        WSlice() : m_buffer(nullptr), m_size(0)
        {}

        WSlice(uint8_t* buffer, size_t size) : m_buffer(buffer), m_size(size)
        {}

        RSlice ToRSlice() const
        {
            return RSlice(m_buffer, m_size);
        }

        void Advance(size_t count)
        {
            const auto NUM = (count > m_size) ? m_size : count;
            m_size -= NUM;
            m_buffer += NUM;
        }

        operator uint8_t* () const
        {
            return m_buffer;
        };

        size_t Size() const
        {
            return m_size;
        }

        private:

        uint8_t* m_buffer;
        size_t m_size;
    };


}



#endif //PROXY_FILEDESC_H
