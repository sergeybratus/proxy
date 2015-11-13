#include "dnp3cpp/DNP3Factory.h"

#include "dnp3hammer/plugin.h"
#include "dnp3cpp/DNP3Parser.h"

namespace proxy { namespace  dnp3 {

DNP3Factory::DNP3Factory()
{
    static bool initialized = false;

    if(!initialized)
    {
        // TODO - determine if there are any global options we need to parameterize
        if(dnp3_dissect_init(NULL) < 0)
        {
            throw std::runtime_error("dnp3 plugin init failed");
        }

        initialized = true;
    }
}

std::unique_ptr<IParser> DNP3Factory::Create(IParserCallbacks &callbacks)
{
    return std::unique_ptr<IParser>(new DNP3Parser(callbacks));
}

}}