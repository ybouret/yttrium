#include "y/utest/run.hpp"
#include "y/net/api.hpp"
#include "y/system/wtime.hpp"

using namespace Yttrium;

Y_UTEST(init)
{
    Network::API & net = Network::API::Instance();
    std::cerr << net.callSign() << std::endl;
    std::cerr << net.hostName   << std::endl;

}
Y_UDONE()

