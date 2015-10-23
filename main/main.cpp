
#include <proxy/ProxyAPI.h>

#include "NullParserPlugin.h"
#include "CommandLineOptions.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace proxy;

std::unique_ptr<IParserFactory> GetFactory(const std::string& name);

int main (int argc, char *argv[])
{
    CommandLineOptions options;
    options.Parse(argc, argv);

    ProxyConfig config;
    std::error_code ec;
    if(!options.Get(config, ec))
    {
        LOG(ERROR) << "Config error: " << ec.message();
        return -1;
    }


    auto factory = GetFactory(options.parser.getValue());

	proxy::Run(config, *factory, ec);
	
	if(ec)
	{
        LOG(ERROR) << "Proxy error: " << ec.message();
	    return -1;
	}
	
	return 0;
}

std::unique_ptr<IParserFactory> GetFactory(const std::string& name)
{
    if(name == "null")
    {
        return std::unique_ptr<IParserFactory>(new NullParserPluginFactory(4096));
    }
    else
    {
        LOG(ERROR) << "Unknown parser: " << name;
        throw std::invalid_argument(name);
    }
}
