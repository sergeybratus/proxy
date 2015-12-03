
#ifndef PROXY_ERRORCODES_H
#define PROXY_ERRORCODES_H

#include <system_error>

namespace proxy
{
    enum class Error
    {
       SERVER_LISTEN_ERROR,      // A previously listening server socket has encountered an error
       EPOLL_SOCKET_ERR,         // epoll returned an error for an open socket
       INVALID_IPV4_ADDRESS,     // Invalid IPv4 address
       END_OF_FILE               // read() returned 0
    };

    class ProxyErrorCategory final : public std::error_category
    {
    public:

        static const std::error_category& Instance()
        {
            return instance;
        }

        virtual const char* name() const noexcept
        {
            return "Proxy errors";
        }

        virtual std::string message(int ev) const;

    private:

        ProxyErrorCategory() {}
        ProxyErrorCategory(const ProxyErrorCategory&) = delete;

        static ProxyErrorCategory instance;
    };

    std::error_code make_error_code(Error err);
}

namespace std
{
    template <>
    struct is_error_code_enum<proxy::Error> : public true_type {};
}

#endif
