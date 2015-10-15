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
    };

}


#endif //PROXY_COMMANDLINEOPTIONS_H
