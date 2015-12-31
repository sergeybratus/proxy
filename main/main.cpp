
#include <proxy/ProxyAPI.h>
#include <proxy/NullParserPlugin.h>
#include <dnp3cpp/DNP3Factory.h>

#include "CommandLineOptions.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace proxy;

/*
    Static buffers for the elfbac policy
    We need some profiling numbers to appropriately size these
    These are sized non-scientifically ATM
*/

// This is set to 2 b/c we need a dissector in each direction
// If we supported simultaneous connections, we'd have to size
// this further
const int NUM_DISSECTOR = 2;

uint8_t input_buff[5*1024*NUM_DISSECTOR];

uint8_t parse_buff[20*1024*1024];

uint8_t context_buff[10*1024*NUM_DISSECTOR];

uint8_t result_buff[3*1024];

// SLOB allocators built on top of the static buffers
static HAllocator *mm_input = nullptr;
static HAllocator *mm_parse = nullptr;
static HAllocator *mm_context = nullptr;
static HAllocator *mm_results = nullptr;

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

    LOG(INFO) << "Using parser: " << options.parser.getValue();

    auto factory = GetFactory(options.parser.getValue());

    if(config.observeOnly)
    {
        LOG(WARNING) << "Proxy is in OBSERVE ONLY mode";
    }

    if(config.exitAfterSession)
    {
        LOG(WARNING) << "Proxy will EXIT after processing a single TCP session";
    }

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
    else if(name == "dnp3")
    {
        dnp3_init();
        LOG(INFO) << "completed dnp3/hammer parser initialization";

        // TODO - how/when do these get freed?
        mm_input = h_sloballoc(input_buff, sizeof(input_buff));
        mm_parse = h_sloballoc(parse_buff, sizeof(parse_buff));
        mm_context = h_sloballoc(context_buff, sizeof(context_buff));
        mm_results = h_sloballoc(result_buff, sizeof(result_buff));

        // TODO  - check these individually
        if(!(mm_input && mm_parse && mm_context && mm_results))
        {
            throw std::logic_error("Allocator init failure");
        }

        LOG(INFO) << "completed SLOB allocator initialization";

        return std::unique_ptr<IParserFactory>(
                new dnp3::DNP3Factory(mm_input, mm_parse, mm_context, mm_results)
        );
    }
    else
    {
        LOG(ERROR) << "Unknown parser: " << name;
        throw std::invalid_argument(name);
    }
}
