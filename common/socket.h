#include <istream>
#include <memory>
#include <netinet/ip.h>

namespace tt
{
    using usize = size_t;

    using fd_t = int;
    using result_t = int;
    constexpr int FAIL = -1;

    class tcp_stream : private std::streambuf, public std::basic_iostream<char>
    {
public:
        [[nodiscard]] static tcp_stream from_fd(fd_t fd);
        [[nodiscard]] static tcp_stream connect(in_addr_t, in_port_t);
        ~tcp_stream();

        void write(std::string s);
        std::string read(usize size);

private:
        tcp_stream(fd_t fd);

        std::unique_ptr<char> _buf;
        fd_t _fd;
    };

    class tcp_listener
    {
public:

        static tcp_listener bind(in_addr_t address = INADDR_ANY, in_port_t port = 8080);
        [[nodiscard]] tcp_stream accept();
        ~tcp_listener();

private:
        tcp_listener(fd_t fd);

        void listen();

        fd_t _fd;
    };

    static void close_and_throw(fd_t fd);

    namespace sockaddr_in
    {
        static ::sockaddr_in from(in_addr_t addr, in_port_t port);
    };
}

