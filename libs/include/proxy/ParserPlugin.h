
#ifndef PROXY_PARSER_PLUGIN_H
#define PROXY_PARSER_PLUGIN_H

#include "proxy/RSlice.h"
#include "proxy/WSlice.h"

#include <functional>
#include <memory>

namespace proxy
{

class IParserCallbacks
{

public:

    virtual void OnErrorMsg(const char *fmt, ...) = 0;

    virtual void OnDebugMsg(const char *fmt, ...) = 0;

    virtual void QueueWrite(const RSlice& data) = 0;
};

class IParser
{
    public:

        // Return a buffer slice that's available for writing
        virtual WSlice GetWriteSlice() = 0;

        // Ask the parser the written portion of its input buffer
        virtual bool Parse(const RSlice& input) = 0;
};

class IParserFactory
{
    public:

        virtual std::unique_ptr<IParser> Create(IParserCallbacks& callbacks) = 0;
};
    
}

#endif
