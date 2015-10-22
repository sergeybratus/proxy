
#include <proxy/ProxyAPI.h>
#include <proxy/NullParserPlugin.h>

#include "CommandLineOptions.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace proxy;

int main (int argc, char *argv[])
{
    CommandLineOptions options;
    options.Parse(argc, argv);

    /// initialize the plugins we'll use
    /// TODO, wrap the DNP3 parser w/ the API and add it to the map
    NullParserPluginFactory nullPluginFactory(4096);

    std::map<std::string, IParserFactory*> parserMap;
    parserMap["null"] = &nullPluginFactory;

    ProxyConfig config;
    std::error_code ec;
    if(!options.Get(config, ec))
    {
        LOG(ERROR) << "Config error: " << ec.message() << std::endl;
        return -1;
    }

    auto parserIter = parserMap.find(options.parser.getValue());

    if(parserIter == parserMap.end())
    {
        LOG(ERROR) << "Unknown parser: " << options.parser.getValue() << std::endl;
        return -1;
    }

	proxy::Run(config, *(parserIter->second), ec);
	
	if(ec)
	{
        LOG(ERROR) << "Proxy error: " << ec.message() << std::endl;
	    return -1;
	}
	
	return 0;
}
