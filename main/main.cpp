
#include <proxy/ProxyAPI.h>

#include "CommandLineOptions.h"

using namespace proxy;


int main (int argc, char *argv[])
{
    CommandLineOptions options;
    options.Parse(argc, argv);

    ProxyConfig config;
    std::error_code ec;
    if(!options.Get(config, ec))
    {
        std::cerr << "Config error: " << ec.message() << std::endl;
        return -1;
    }

	proxy::Run(config, ec);
	
	if(ec)
	{
	  std::cerr << "Proxy error: " << ec.message() << std::endl;
	  return -1;
	}
	
	return 0;
}
