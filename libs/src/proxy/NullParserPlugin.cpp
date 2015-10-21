
#include "proxy/NullParserPlugin.h"

namespace proxy
{

NullParserPlugin::NullParserPlugin(const QueueWriteFun& write_) :  write(write_)
{}

bool NullParserPlugin::Feed(const uint8_t* data, size_t len)
{
    this->write(data, len);
    return true;
}

NullParserPluginFactory NullParserPluginFactory::instance;

std::unique_ptr<IParser> NullParserPluginFactory::Create(const QueueWriteFun& write)
{
    return std::unique_ptr<IParser>(new NullParserPlugin(write));
}

}