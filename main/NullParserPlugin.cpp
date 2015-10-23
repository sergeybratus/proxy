
#include "NullParserPlugin.h"

namespace proxy
{

NullParserPlugin::NullParserPlugin(size_t bufferSize, IParserCallbacks& callbacks_) :
        m_buffer(bufferSize),
        m_callbacks(callbacks_)
{}

bool NullParserPlugin::Parse(const RSlice& data)
{
    m_callbacks.QueueWrite(data);
    return true;
}

WSlice NullParserPlugin::GetWriteSlice()
{
    return m_buffer.GetWSlice();
}

NullParserPluginFactory::NullParserPluginFactory(size_t bufferSize) : M_BUFFER_SIZE(bufferSize)
{

}

std::unique_ptr<IParser> NullParserPluginFactory::Create(IParserCallbacks& callbacks)
{
    return std::unique_ptr<IParser>(new NullParserPlugin(M_BUFFER_SIZE, callbacks));
}

}