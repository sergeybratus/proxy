
#include <proxy/ProxyAPI.h>

#include <iostream>

using namespace proxy;


int main (int argc, char *argv[])
{
	std::vector<SessionConfig> config;

    std::error_code ec;

	SessionConfig session1;
	session1.client.port = 8080;

    if(!session1.client.SetAddress("127.0.0.1", ec))
    {
        std::cerr << "Config error: " << ec.message() << std::endl;
        return -1;
    }

    config.push_back(session1);

	proxy::Run(config, ec);
	
	if(ec)
	{
	  std::cerr << "Proxy error: " << ec.message() << std::endl;
	  return -1;
	}
	
	return 0;
}
