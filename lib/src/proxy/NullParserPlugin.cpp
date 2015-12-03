
#include "proxy/NullParserPlugin.h"

namespace proxy
{

NullParserPlugin::NullParserPlugin(size_t bufferSize, IParserCallbacks& callbacks_) :
        m_buffer(bufferSize),
        m_callbacks(callbacks_)
{}

bool NullParserPlugin::Parse(size_t num)
{
    m_callbacks.QueueWrite(m_buffer.ToRSlice().Take(num));
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