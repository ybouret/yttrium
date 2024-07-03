#include "y/utest/run.hpp"
#include "y/net/api.hpp"
#include "y/system/wtime.hpp"

using namespace Yttrium;

Y_UTEST(init)
{
    Network &net = Network::Instance();
    std::cerr << net.callSign() << std::endl;


}
Y_UDONE()

