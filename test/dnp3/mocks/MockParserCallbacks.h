//
// Created by user on 11/2/15.
//

#ifndef PROXY_MOCKPARSERCALLBACKS_H
#define PROXY_MOCKPARSERCALLBACKS_H

#include <proxy/ProxyAPI.h>

namespace proxy {

    class MockParserCallbacks final : public proxy::IParserCallbacks {

        virtual void OnErrorMsg(const char *fmt, ...) override {}

        virtual void OnDebugMsg(const char *fmt, ...) override {}

        virtual void QueueWrite(const RSlice &data) override {}

    };

}

#endif //PROXY_MOCKPARSERCALLBACKS_H
