
#include <proxy/ProxyAPI.h>

#include "CommandLineOptions.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace proxy;

int main (int argc, char *argv[])
{
    CommandLineOptions options;
    options.Parse(argc, argv);

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
