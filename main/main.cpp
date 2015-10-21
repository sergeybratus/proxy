
#include <proxy/ProxyAPI.h>
#include <proxy/ParserPlugin.h>
#include <proxy/NullParserPlugin.h>

#include "CommandLineOptions.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace proxy;

int main (int argc, char *argv[])
{
    std::map<std::string, IParserFactory*> parserMap;
    parserMap["null"] = &NullParserPluginFactory::Instance();

    CommandLineOptions options;
    options.Parse(argc, argv);

    auto parserIter = parserMap.find(options.parser.getValue());

    if(parserIter == parserMap.end())
    {
        std::cerr << "Unknown parser: " << options.parser.getValue() << std::endl;
        return -1;
    }

    ProxyConfig config;
    std::error_code ec;
    if(!options.Get(config, ec))
    {
        LOG(ERROR) << "Config error: " << ec.message() << std::endl;
        return -1;
    }

	proxy::Run(config, ec);
	
	if(ec)
	{
        LOG(ERROR) << "Proxy error: " << ec.message() << std::endl;
	    return -1;
	}
	
	return 0;
}
