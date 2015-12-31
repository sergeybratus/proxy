#include "dnp3cpp/DNP3Parser.h"

#include <easylogging++.h>
#include <dnp3hammer.h>

namespace proxy { namespace  dnp3 {

DNP3Parser::DNP3Parser(HAllocator *mm_input,
                       HAllocator *mm_parse,
                       HAllocator *mm_context,
                       HAllocator *mm_results,
                       SessionDir dir,
                       IParserCallbacks& callbacks) :

        m_dir(dir == SessionDir::ClientToServer ? " -c-> " : " <-s- "),
        m_callbacks(callbacks),
        m_plugin(dnp3_dissector__m(mm_input, mm_parse, mm_context, mm_results, GetCallbacks(), this)),
        m_parse_valid(true)
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
    m_parse_valid = true;

    auto res = m_plugin->feed(m_plugin, num);

    if(res)
    {
        LOG(ERROR) << "plugin feed() failed";
    }

    // the underlying parser encountered no errors and no error callbacks occurred
    return !res && m_parse_valid;
}

DNP3_Callbacks DNP3Parser::GetCallbacks()
{
    return DNP3_Callbacks {
            .link_invalid = &DNP3Parser::OnLinkInvalid,
            .link_frame = &DNP3Parser::OnLinkFrame,
            .transport_segment = &DNP3Parser::OnTransportSegment,
            .transport_discard = &DNP3Parser::OnTransportDiscard,
            .transport_payload = &DNP3Parser::OnTransportPayload,
            .app_invalid = &DNP3Parser::OnAppInvalid,
            .app_fragment = &DNP3Parser::OnAppFragment,
            .log_error = &DNP3Parser::OnLogError
    };
}

int DNP3Parser::OnLinkFrame(void *env, const DNP3_Frame *frame, const uint8_t *buf, size_t len)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);

    // TODO - correct log level - should we use the formatting function here?
    char* output = dnp3_format_frame(frame);
    LOG(INFO) << parser->m_dir << output;
    free(output);

    if(frame->func == DNP3_UNCONFIRMED_USER_DATA ||
       frame->func == DNP3_CONFIRMED_USER_DATA) {
        return 0;
    }

    parser->m_callbacks.QueueWrite(RSlice(buf, len));
    return 0;
}

void DNP3Parser::OnLinkInvalid(void *env, const DNP3_Frame *frame)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);

    parser->m_parse_valid = false;

    LOG(ERROR) << "invalid link layer frame";
}

void DNP3Parser::OnTransportSegment(void *env, const DNP3_Segment *segment)
{
    // -- TODO -- logging?
}

void DNP3Parser::OnTransportDiscard(void *env, size_t n)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);

    parser->m_parse_valid = false;

    LOG(ERROR) << "Transport segment discarded w/ length: " << n;
}

void DNP3Parser::OnTransportPayload(void *env, const uint8_t *s, size_t n)
{
    // -- TODO -- logging?
}

void DNP3Parser::OnAppInvalid(void *env, DNP3_ParseError e)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);

    parser->m_parse_valid = false;

    LOG(ERROR) << "DNP3 parser error: " << e << " (" << ParseErrorToString(e) << ")";
}

void DNP3Parser::OnAppFragment(void *env, const DNP3_Fragment *fragment, const uint8_t *buf, size_t len)
{
    auto parser = reinterpret_cast<DNP3Parser*>(env);

    // TODO - correct log level- can we get a lighter level of inspection
    // and make doing this dependent on logging being enabled?
    char* output = dnp3_format_fragment_ohdrs(fragment);
    LOG(INFO) << parser->m_dir << output;
    free(output);

    parser->m_callbacks.QueueWrite(RSlice(buf, len));
}

void DNP3Parser::OnLogError(void *env, const char *fmt, ...)
{
    char output[80];

    va_list args;
    va_start(args, fmt);
    snprintf(output, sizeof(output), fmt, args);
    va_end(args);

    LOG(ERROR) << "DNP3 parser error: " << output;
}

const char* DNP3Parser::ParseErrorToString(DNP3_ParseError pe)
{
    switch(pe)
    {
        case(ERR_FUNC_NOT_SUPP):
            return "Function not supported";
        case(ERR_OBJ_UNKNOWN):
            return "Object unknown";
        case(ERR_PARAM_ERROR):
            return "Paramter error";
        default:
            return "Incomplete application header";
    }
}

}}

