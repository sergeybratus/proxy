
#ifndef PROXYDNP3_DNP3FACTORY_H
#define PROXYDNP3_DNP3FACTORY_H

#include <plugin/ParserPlugin.h>

namespace proxy { namespace  dnp3 {

    class DNP3Factory final : public IParserFactory
    {

    public:

        DNP3Factory();

        virtual std::unique_ptr<IParser> Create(IParserCallbacks& callbacks) override;
    };

}}

#endif //PROXY_DNP3FACTORY_H
