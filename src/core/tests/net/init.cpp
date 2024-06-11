
#include "y/utest/run.hpp"
#include "y/net/api.hpp"


using namespace Yttrium;

Y_UTEST(net_init)
{
	Network &net = Network::Instance();
	std::cerr << net.callSign() << std::endl;
}
Y_UDONE()

