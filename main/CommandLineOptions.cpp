
#include "CommandLineOptions.h"

namespace proxy
{

    CommandLineOptions::CommandLineOptions() :
            cmd("dnp3proxy", ' ', "0.1"),
            listenPort("l", "listenPort", "Port the server listens on", false, 20000, "tcp port"),
            listenAdapter("a", "listenAdapter", "Adapter on which the server listens for connection", false,
                          "127.0.0.1", "ipv4 address") {

    }

}