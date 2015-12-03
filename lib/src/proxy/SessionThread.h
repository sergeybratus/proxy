

#ifndef PROXY_SESSIONTHREAD_H
#define PROXY_SESSIONTHREAD_H

#include "proxy/ProxySession.h"

#include <thread>


namespace proxy
{

    /// Handles a single session
    class SessionThread : private Uncopyable
    {
        public:

            SessionThread(const EndpointConfig& config, FileDesc& server_fd, IParserFactory& factory);

        private:

            void Run();

            ProxySession m_session;
            std::thread m_thread;
    };

}

#endif
