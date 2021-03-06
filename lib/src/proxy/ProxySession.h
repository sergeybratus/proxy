

#ifndef PROXY_PROXYSESSION_H
#define PROXY_PROXYSESSION_H

#include <plugin/ParserPlugin.h>
#include <proxy/ProxyConfig.h>

#include "proxy/FileDesc.h"

#include <deque>
#include <system_error>


namespace proxy
{

    /// Handles a single session
    class ProxySession : private IParserCallbacks
    {
        public:

            ProxySession(const ProxyConfig& config, FileDesc& server_fd, IParserFactory& factory);

            void Run(std::error_code& ec);

        private:

            /// ---- implement IParserCallbacks ----

            virtual void QueueWrite(const RSlice& output) override;

            /// ---- private members ----

            static bool RegisterForDataAvailable(const FileDesc& epoll_fd, const FileDesc& fd, std::error_code &ec);
            static bool Modify(const FileDesc& epoll_fd, int operation, const FileDesc& fd, uint32_t events, std::error_code &ec);

            FileDesc Connect(std::error_code& ec);

            bool RunOne(FileDesc& epoll_fd, FileDesc& client_fd, std::error_code &ec);
            bool Transfer(FileDesc& src, FileDesc& dest, IParser& parser, std::error_code &ec);

            bool WriteOutputTo(FileDesc& dest, std::error_code &ec);

            static bool WriteAll(FileDesc& dest, const RSlice& data, std::error_code &ec);

            /// ---- private members ----

            const ProxyConfig m_config;
            FileDesc m_server_fd;

            // a parser for each direction of the stream
            std::unique_ptr<IParser> m_s2cParser;
            std::unique_ptr<IParser> m_c2sParser;

            std::deque<RSlice> m_output_vec;
    };

}

#endif //PROXY_PROXYSESSION_H
