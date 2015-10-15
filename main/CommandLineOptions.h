#ifndef PROXY_COMMANDLINEOPTIONS_H
#define PROXY_COMMANDLINEOPTIONS_H

#include <tclap/CmdLine.h>

namespace proxy  {

    class CommandLineOptions
    {

    public:
        CommandLineOptions();

        void Parse(int argc, char** argv);

    private:
        TCLAP::CmdLine cmd;

    public:

        TCLAP::ValueArg<uint16_t> listenPort;
        TCLAP::ValueArg<std::string> listenAdapter;

        TCLAP::ValueArg<uint16_t> connectPort;
        TCLAP::ValueArg<std::string> connectHost;
    };

}


#endif //PROXY_COMMANDLINEOPTIONS_H
