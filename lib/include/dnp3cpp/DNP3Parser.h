
#ifndef PROXYDNP3_DNP3PARSER_H
#define PROXYDNP3_DNP3PARSER_H

#include <plugin/ParserPlugin.h>
#include <plugin/Uncopyable.h>

#include <dnp3hammer.h>

namespace proxy { namespace  dnp3 {

    class DNP3Parser final : public IParser, private Uncopyable {

            friend class DNP3Factory;

            DNP3Parser(HAllocator *mm_input,
                       HAllocator *mm_parse,
                       HAllocator *mm_context,
                       HAllocator *mm_results,
                       SessionDir dir,
                       IParserCallbacks& callbacks);

        public:

            ~DNP3Parser();

            virtual WSlice GetWriteSlice() override;

            virtual bool Parse(size_t num) override;

        private:


            static DNP3_Callbacks GetCallbacks();

            static int  OnLinkFrame(void *env, const DNP3_Frame *frame, const uint8_t *buf, size_t len);
            static void OnLinkInvalid(void *env, const DNP3_Frame *frame);
            static void OnTransportSegment(void *env, const DNP3_Segment *segment);
            static void OnTransportDiscard(void *env, size_t n);
            static void OnTransportPayload(void *env, const uint8_t *s, size_t n);
            static void OnAppInvalid(void *env, DNP3_ParseError e);
            static void OnAppFragment(void *env, const DNP3_Fragment *fragment, const uint8_t *buf, size_t len);
            static void OnLogError(void *env, const char *fmt, ...);

            static const char* ParseErrorToString(DNP3_ParseError pe);

            const char* m_dir;
            IParserCallbacks& m_callbacks;
            StreamProcessor* m_plugin;

            // this is a bit of a hack since the underlying parser doesn't return an error code
            // we track the state of the parse using this flag
            bool m_parse_valid;
    };

}}


#endif //PROXY_DNP3PARSER_H
