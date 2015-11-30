#include "dnp3cpp/DNP3Parser.h"


/*
void hook_link_frame(DissectPlugin *self, const DNP3_Frame *frame, const uint8_t *buf, size_t len)
{}

void hook_transport_reject(DissectPlugin *self)
{}

void hook_transport_segment(DissectPlugin *self, const DNP3_Segment *segment)
{}

void hook_transport_payload(DissectPlugin *self, const uint8_t *s, size_t n)
{}

void hook_app_reject(DissectPlugin *self)
{}

void hook_app_error(DissectPlugin *self, DNP3_ParseError e)
{}

void hook_app_fragment(DissectPlugin *self, const DNP3_Fragment *fragment, const uint8_t *buf, size_t len)
{}
*/

namespace proxy { namespace  dnp3 {

WSlice DNP3Parser::GetWriteSlice()
{
    return WSlice(m_plugin->buf, m_plugin->bufsize);
}

bool DNP3Parser::Parse(size_t num)
{
    return m_plugin->feed(m_plugin, num);
}

void DNP3Parser::QueueOutput(void *env, const uint8_t *buf, size_t n)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);
    parser->m_callbacks.QueueWrite(RSlice(buf, n));
}

DNP3Parser::DNP3Parser(IParserCallbacks& callbacks) :
        m_callbacks(callbacks),
        m_plugin(nullptr)
{
    if(m_plugin == nullptr)
    {
        throw std::runtime_error("DNP3 plugin bind failed");
    }
}

DNP3Parser::~DNP3Parser()
{
    if(m_plugin)
    {
        m_plugin->finish(m_plugin);
    }
}

}}

