#include "y/utest/run.hpp"
#include "y/net/api.hpp"
#include "y/system/wtime.hpp"

#if defined(Y_BSD)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

using namespace Yttrium;

Y_UTEST(init)
{
    Network::API & net = Network::API::Instance();
    std::cerr << net.callSign() << std::endl;
    std::cerr << net.hostName   << std::endl;
    net.verbose = true;

    {
        Network::SystemSocket s = net.socket_create(PF_INET,SOCK_STREAM,IPPROTO_TCP);
        net.socket_delete(s);
    }

    {
        Network::SystemSocket s = net.socket_create(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
        net.socket_delete(s);
    }


}
Y_UDONE()

