
#ifndef PROXY_RSLICE_H
#define PROXY_RSLICE_H

#include <cstdint>
#include <cstddef>

namespace proxy
{
    // A readonly "slice" of a buffer
    class RSlice
    {

        public:

        RSlice() : m_buffer(nullptr), m_size(0)
        {}

        RSlice(uint8_t const* buffer, size_t size) : m_buffer(buffer), m_size(size)
        {}

        RSlice Take(size_t count) const
        {
            const auto NUM = (count > m_size) ? m_size : count;
            return  RSlice(m_buffer, NUM);
        }

        void Advance(size_t count)
        {
            const auto NUM = (count > m_size) ? m_size : count;
            m_size -= NUM;
            m_buffer += NUM;
        }

        operator uint8_t const* () const
        {
            return m_buffer;
        };

        size_t Size() const
        {
            return m_size;
        }

        bool IsEmpty() const
        {
            return m_size == 0;
        }

        private:

        uint8_t const* m_buffer;
        size_t m_size;
    };


}



#endif //PROXY_FILEDESC_H
