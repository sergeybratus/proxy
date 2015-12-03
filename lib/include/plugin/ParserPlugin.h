
#ifndef PARSER_PLUGIN_H
#define PARSER_PLUGIN_H

#include "RSlice.h"
#include "WSlice.h"

#include <functional>
#include <memory>

namespace proxy
{

class IParserCallbacks
{

public:

    virtual void QueueWrite(const RSlice& data) = 0;
};

class IParser
{
    public:

        virtual ~IParser() {}

        // Return a buffer slice that's available for writing
        virtual WSlice GetWriteSlice() = 0;

        // Ask the parser to process the written portion of its input buffer
        // return false if the parser violates some kind of
        virtual bool Parse(size_t num) = 0;
};

class IParserFactory
{
    public:

        virtual ~IParserFactory() {}

        virtual std::unique_ptr<IParser> Create(IParserCallbacks& callbacks) = 0;
};
    
}

#endif
