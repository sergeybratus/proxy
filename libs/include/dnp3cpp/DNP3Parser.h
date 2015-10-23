
#ifndef PROXY_DNP3PARSER_H
#define PROXY_DNP3PARSER_H

#include <plugin/ParserPlugin.h>
#include <plugin/Uncopyable.h>

// forward declare the C-struct
typedef struct Plugin_ Plugin;

namespace proxy { namespace  dnp3 {

    class DNP3Parser final : public IParser, private Uncopyable {

            DNP3Parser(IParserCallbacks& callbacks);
            ~DNP3Parser();

        public:

            virtual WSlice GetWriteSlice() override;

            virtual bool Parse(size_t num) override;

        private:

            static void QueueOutput(void *env, const uint8_t *buf, size_t n);




            IParserCallbacks& m_callbacks;
            Plugin* m_plugin;
    };

}}


#endif //PROXY_DNP3PARSER_H
