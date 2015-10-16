
#include "CommandLineOptions.h"

namespace proxy
{

    CommandLineOptions::CommandLineOptions() :
            cmd("dnp3proxy", ' ', "0.1"),
            listenPort("l", "listenPort", "Port the server listens on", false, 20000, "tcp port"),
            listenAdapter("a", "listenAdapter", "Adapter on which the server listens for connection", false, "127.0.0.1", "ipv4 address"),
            // we make this 20001, just so the default configuration doesn't connect to itself
            connectPort("c", "connPort", "Port on which to connect to the downstream server", false, 20001, "tcp port"),
            connectHost("n", "connAdapter", "IP address of the downstream host", false, "127.0.0.1", "ipv4 address")
    {
        cmd.add(listenPort);
        cmd.add(listenAdapter);
        cmd.add(connectPort);
        cmd.add(connectHost);
    }

    void CommandLineOptions::Parse(int argc, char** argv)
    {
        cmd.parse(argc, argv);
    }

    bool CommandLineOptions::Get(ProxyConfig& config, std::error_code& ec)
    {
        config.client.port = connectPort.getValue();
        config.server.port = listenPort.getValue();

        config.client.SetAddress(connectHost.getValue(), ec);

        if(ec)
        {
            return false;
        }

        config.server.SetAddress(listenAdapter.getValue(), ec);

        if(ec)
        {
            return false;
        }

        return true;
    }
}