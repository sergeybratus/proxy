#ifndef PROXY_NULLPARSERPLUGIN_H
#define PROXY_NULLPARSERPLUGIN_H

#include "proxy/ParserPlugin.h"

namespace proxy {

    class NullParserPlugin final : public IParser
    {
        public:

        NullParserPlugin(const QueueWriteFun& write);

        virtual bool Feed(const uint8_t*, size_t len) override;

        private:

        QueueWriteFun write;
    };

    class NullParserPluginFactory final : public IParserFactory
    {
    public:

        // return a unique name for the plugin
        virtual std::string Name() const override { return "null"; }

        static IParserFactory& Instance() { return instance; }

    private:

        virtual std::unique_ptr<IParser> Create(const QueueWriteFun& write) override;

        NullParserPluginFactory() {}
        NullParserPluginFactory(const NullParserPluginFactory&) = delete;

        static NullParserPluginFactory instance;
    };

}

#endif //PROXY_NULLPARSERPLUGIN_H
