
#ifndef PROXY_RSLICE_H
#define PROXY_RSLICE_H

#include <cstdint>
#include <cstddef>

namespace proxy
{
    class WSlice;

    // A readonly "slice" of a buffer
    class RSlice
    {

        public:

        RSlice();
        RSlice(uint8_t const* buffer, size_t size);

        RSlice Take(size_t count) const;

        RSlice CopyTo(WSlice& dest) const;

        void Advance(size_t count);

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
