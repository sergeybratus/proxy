
#include <proxy/Proxy.h>

#include <iostream>

using namespace proxy;

int main (int argc, char *argv[])
{
	std::vector<SessionConfig> config;
	
	SessionConfig config1;
	//config1.listening.address.s_addr = ~0;
	SessionConfig config2;
	config.push_back(config1);
	//config.push_back(config2);
  
	Proxy::Run(config);		
	
	return 0;
}
