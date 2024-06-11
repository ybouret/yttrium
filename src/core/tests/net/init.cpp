
#include "y/utest/run.hpp"
#include "y/net/api.hpp"
#include "y/system/wtime.hpp"

using namespace Yttrium;

Y_UTEST(net_init)
{
	Network &net = Network::Instance();
	std::cerr << net.callSign() << std::endl;

    std::cerr << "Sleeping..." << std::endl;
    WallTime       tmx;
    const uint64_t mark = WallTime::Ticks();
    //net.sleepFor(1.5);
    const double   ell = tmx( WallTime::Ticks() - mark );
    std::cerr << "Bye after " << ell << "s!" << std::endl;
}
Y_UDONE()

