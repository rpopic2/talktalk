#include <iostream>
#include <netinet/in.h>
#include <vector>
#include "../common/socket.h"

int main()
{
    auto listener = tt::tcp_listener::bind(INADDR_ANY, 8080);
    auto client = listener.accept();

    auto recv = client.read(2);
    std::cout << recv << std::endl;
}

