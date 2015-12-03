#include "dnp3cpp/DNP3Factory.h"

#include "dnp3cpp/DNP3Parser.h"
#include <easylogging++.h>

namespace proxy { namespace  dnp3 {

DNP3Factory::DNP3Factory()
{
    static bool initialized = false;

    if(!initialized)
    {
	    dnp3_init();
        LOG(INFO) << "completed dnp3/hammer parser initialization";
        initialized = true;
    }
}

std::unique_ptr<IParser> DNP3Factory::Create(SessionDir dir, IParserCallbacks& callbacks)
{
    return std::unique_ptr<IParser>(new DNP3Parser(dir, callbacks));
}

}}
