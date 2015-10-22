
#include "proxy/NullParserPlugin.h"

namespace proxy
{

NullParserPlugin::NullParserPlugin(IParserCallbacks& callbacks_) :  callbacks(callbacks_)
{}

bool NullParserPlugin::Feed(const uint8_t* data, size_t len)
{
    this->callbacks.QueueWrite(data, len);
    return true;
}

NullParserPluginFactory NullParserPluginFactory::instance;

std::unique_ptr<IParser> NullParserPluginFactory::Create(IParserCallbacks& callbacks)
{
    return std::unique_ptr<IParser>(new NullParserPlugin(callbacks));
}

}