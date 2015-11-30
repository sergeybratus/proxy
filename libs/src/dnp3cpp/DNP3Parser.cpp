#include "dnp3cpp/DNP3Parser.h"

namespace proxy { namespace  dnp3 {

DNP3Parser::DNP3Parser(IParserCallbacks& callbacks) :
        m_callbacks(callbacks),
        m_plugin(dnp3_dissector(GetCallbacks(), this))
{
    if(m_plugin == nullptr)
    {
        throw std::runtime_error("dnp3_dissector init failure");
    }
}

DNP3Parser::~DNP3Parser()
{
    if(m_plugin)
    {
        m_plugin->finish(m_plugin);
    }
}

WSlice DNP3Parser::GetWriteSlice()
{
    return WSlice(m_plugin->buf, m_plugin->bufsize);
}

bool DNP3Parser::Parse(size_t num)
{
    return m_plugin->feed(m_plugin, num);
}

DNP3_Callbacks DNP3Parser::GetCallbacks()
{
    return DNP3_Callbacks {
            .link_frame = &DNP3Parser::OnLinkFrame,
            .transport_segment = &DNP3Parser::OnTransportSegment,
            .transport_payload = &DNP3Parser::OnTransportPayload,
            .app_invalid = &DNP3Parser::OnAppInvalid,
            .app_fragment = &DNP3Parser::OnAppFragment
    };
}

void DNP3Parser::OnLinkFrame(void *env, const DNP3_Frame *frame, const uint8_t *buf, size_t len)
{
    if(frame->func == DNP3_UNCONFIRMED_USER_DATA ||
       frame->func == DNP3_CONFIRMED_USER_DATA) {
        return;
    }

    reinterpret_cast<DNP3Parser*>(env)->m_callbacks.QueueWrite(RSlice(buf, len));
}

void DNP3Parser::OnTransportSegment(void *env, const DNP3_Segment *segment)
{

}

void DNP3Parser::OnTransportPayload(void *env, const uint8_t *s, size_t n)
{

}

void DNP3Parser::OnAppInvalid(void *env, DNP3_ParseError e)
{

}

void DNP3Parser::OnAppFragment(void *env, const DNP3_Fragment *fragment, const uint8_t *buf, size_t len)
{
    reinterpret_cast<DNP3Parser*>(env)->m_callbacks.QueueWrite(RSlice(buf, len));
}

void DNP3Parser::OnLogError(void *env, const char *fmt, ...)
{

}

}}

