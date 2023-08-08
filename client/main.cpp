#include "../common/socket.h"
#include <netinet/in.h>

int main()
{
    auto sock = tt::tcp_stream::connect(INADDR_LOOPBACK, 8080);
    sock.write("Hello World!");
}
