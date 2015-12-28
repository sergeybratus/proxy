
#ifndef PROXYDNP3_DNP3FACTORY_H
#define PROXYDNP3_DNP3FACTORY_H

#include <plugin/ParserPlugin.h>

namespace proxy { namespace  dnp3 {

    class DNP3Factory final : public IParserFactory
    {

    public:

        DNP3Factory(const WSlice& input_buffer);

        virtual std::unique_ptr<IParser> Create(SessionDir dir, IParserCallbacks& callbacks) override;

    private:

        WSlice m_input_buffer;
    };

}}

#endif //PROXY_DNP3FACTORY_H
