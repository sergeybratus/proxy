
#ifndef PROXY_ERRORCODES_H
#define PROXY_ERRORCODES_H

#include <system_error>

namespace proxy
{
    enum class Error
    {
       SERVER_LISTEN_ERROR,     // A previously listening server socket has encountered an error
       UNKNOWN_SESSION_EVENT,   // An unknown event occurred on a proxy session
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
