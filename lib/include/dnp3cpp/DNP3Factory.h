
#ifndef PROXYDNP3_DNP3FACTORY_H
#define PROXYDNP3_DNP3FACTORY_H

#include <plugin/ParserPlugin.h>

#include <dnp3hammer.h>

namespace proxy { namespace  dnp3 {

    class DNP3Factory final : public IParserFactory
    {

    public:

        DNP3Factory( HAllocator *mm_input,
                     HAllocator *mm_parse,
                     HAllocator *mm_context,
                     HAllocator *mm_results);

        virtual std::unique_ptr<IParser> Create(SessionDir dir, IParserCallbacks& callbacks) override;

    private:

        HAllocator *mm_input;
        HAllocator *mm_parse;
        HAllocator *mm_context;
        HAllocator *mm_results;
    };

}}

#endif //PROXY_DNP3FACTORY_H
