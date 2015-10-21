
#ifndef PROXY_PARSER_PLUGIN_H
#define PROXY_PARSER_PLUGIN_H

#include <functional>
#include <memory>

namespace proxy
{

using QueueWriteFun = std::function<void (const uint8_t*, size_t len)>;

class IParser
{
    public:
        virtual bool Feed(const uint8_t*, size_t len) = 0;
};

class IParserFactory
{
        virtual std::string Name() const = 0;

        virtual std::unique_ptr<IParser> Create(const QueueWriteFun& write) = 0;
};
    
}

#endif
