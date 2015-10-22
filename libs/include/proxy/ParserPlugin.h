
#ifndef PROXY_PARSER_PLUGIN_H
#define PROXY_PARSER_PLUGIN_H

#include <functional>
#include <memory>

namespace proxy
{

class IParserCallbacks
{

public:
    virtual void OnErrorMsg(const char *fmt, ...) = 0;
    virtual void OnDebugMsg(const char *fmt, ...) = 0;

    virtual void QueueWrite(const uint8_t*, size_t len) = 0;
};

class IParser
{
    public:
        virtual bool Feed(const uint8_t*, size_t len) = 0;
};

class IParserFactory
{
        virtual std::string Name() const = 0;

        virtual std::unique_ptr<IParser> Create(IParserCallbacks& callbacks) = 0;
};
    
}

#endif
