
#ifndef PROXYDNP3_DNP3PARSER_H
#define PROXYDNP3_DNP3PARSER_H

#include <plugin/ParserPlugin.h>
#include <plugin/Uncopyable.h>

extern "C"
{
    // forward declare the C-struct
    typedef struct Plugin_ Plugin;

    void error(const char *fmt, ...);
    void debug_(const char *fmt, ...);

}


namespace proxy { namespace  dnp3 {

    class DNP3Parser final : public IParser, private Uncopyable {

            friend class DNP3Factory;

            DNP3Parser(IParserCallbacks& callbacks);

        public:

            ~DNP3Parser();

            virtual WSlice GetWriteSlice() override;

            virtual bool Parse(size_t num) override;

        private:

            static void QueueOutput(void *env, const uint8_t *buf, size_t n);

            IParserCallbacks& m_callbacks;
            Plugin* m_plugin;
    };

}}


#endif //PROXY_DNP3PARSER_H
