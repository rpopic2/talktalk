#include <cstdio>
#include <cstring>
#include <errno.h>
#include <istream>
#include <memory>
#include <netinet/ip.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "socket.h"

using namespace tt;

tcp_stream tcp_stream::from_fd(fd_t fd)
{
    return tcp_stream(fd);
}

tcp_stream tcp_stream::connect(in_addr_t address, in_port_t port)
{
    struct sockaddr_in addr;
    fd_t fd;
    result_t result;

    addr = sockaddr_in::from(address, port);
    fd = socket(PF_INET, SOCK_STREAM, 0);
    result = ::connect(fd, (sockaddr *)&addr, sizeof(addr));

    if (result == FAIL) {
        close_and_throw(fd);
    }

    return tcp_stream::from_fd(fd);
}

tcp_stream::tcp_stream(fd_t fd) : std::basic_iostream<char>(this)
{
    _fd = fd;
    _buf = std::make_unique<char>(10);
}

tcp_stream::~tcp_stream()
{
    close(_fd);
}

void tcp_stream::write(std::string s)
{
    ::write(_fd, s.c_str(), s.size());
    int a = 3;
    int &ref = a;

    int *p = &ref;
}

std::string tcp_stream::read(usize size)
{
    char *buf = _buf.get();
    ::read(_fd, buf, size);
    return std::string(buf);
}

tcp_listener tcp_listener::bind(in_addr_t address, in_port_t port)
{
    struct sockaddr_in addr;
    fd_t fd;
    result_t result;

    addr = sockaddr_in::from(address, port);
    fd = socket(PF_INET, SOCK_STREAM, 0);
    result = ::bind(fd, (sockaddr *)&addr, sizeof(addr));

    if (result == FAIL) {
        close_and_throw(fd);
        return -1;
    }

    result = ::listen(fd, 0);

    return tcp_listener(fd);
}

tcp_listener::tcp_listener(fd_t fd)
{
    _fd = fd;
}

tcp_listener::~tcp_listener()
{
    close(_fd);
}

tcp_stream tcp_listener::accept()
{
    fd_t client;

    client = ::accept(_fd, NULL, NULL);
    if (client == FAIL) {
        close_and_throw(client);
    }
    return tcp_stream::from_fd(client);
}

::sockaddr_in tt::sockaddr_in::from(in_addr_t addr, in_port_t port)
{
    ::sockaddr_in sockaddr;

    memset(&addr, 0, sizeof(addr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(8080);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    return sockaddr;
}

void tt::close_and_throw(fd_t fd)
{
    close(fd);
    throw std::runtime_error(std::strerror(errno));
}
