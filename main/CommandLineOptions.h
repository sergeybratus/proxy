#ifndef PROXY_COMMANDLINEOPTIONS_H
#define PROXY_COMMANDLINEOPTIONS_H

#include <tclap/CmdLine.h>

#include <proxy/ProxyConfig.h>

namespace proxy  {

    class CommandLineOptions
    {

    public:
        CommandLineOptions();

        void Parse(int argc, char** argv);

        bool Get(ProxyConfig& config, std::error_code& ec);

        TCLAP::CmdLine cmd;

        TCLAP::ValueArg<uint16_t> listenPort;
        TCLAP::ValueArg<std::string> listenAdapter;

        TCLAP::ValueArg<uint16_t> connectPort;
        TCLAP::ValueArg<std::string> connectHost;

        TCLAP::ValueArg<std::string> parser;
    };

}


#endif //PROXY_COMMANDLINEOPTIONS_H
