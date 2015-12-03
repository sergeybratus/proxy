
#include "plugin/RSlice.h"

#include "plugin/WSlice.h"

#include <cstring>

namespace proxy {

RSlice::RSlice() : m_buffer(nullptr), m_size(0)
{}

RSlice::RSlice(uint8_t const* buffer, size_t size) : m_buffer(buffer), m_size(size)
{}

RSlice RSlice::Take(size_t count) const
{
    const auto NUM = (count > m_size) ? m_size : count;
    return  RSlice(m_buffer, NUM);
}

RSlice RSlice::CopyTo(WSlice& dest) const
{
    if (dest.Size() < m_size)
    {
        return RSlice();
    }
    else
    {
        WSlice copy(dest);
        memcpy(dest, m_buffer, m_size);
        dest.Advance(m_size);
        return copy.ToRSlice().Take(m_size);
    }
}

void RSlice::Advance(size_t count)
{
    const auto NUM = (count > m_size) ? m_size : count;
    m_size -= NUM;
    m_buffer += NUM;
}

}
