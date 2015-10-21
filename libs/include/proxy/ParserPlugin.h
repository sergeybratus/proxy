
#ifndef PROXY_PARSER_PLUGIN_H
#define PROXY_PARSER_PLUGIN_H

#include <functional>
#include <memory>

namespace proxy
{

using WriteFun = std::function<void (const uint8_t*, size_t len)>;

class IStreamParser
{
    public:
        virtual void Feed(const uint8_t*, size_t len) = 0;
};

class IStreamParserFactory
{
        virtual std::unique_ptr<IStreamParser> Create(const WriteFun& write) = 0;
};
    
}

#endif
