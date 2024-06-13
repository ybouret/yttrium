#include "y/system/sleep.hpp"
#include "y/utest/run.hpp"
#include "y/system/wtime.hpp"

using namespace Yttrium;

Y_UTEST(eta)
{
    WallTime tmx;
    const uint64_t mark = WallTime::Ticks();
    SystemSleep::For(50);
    const double   ell = tmx(WallTime::Ticks() - mark);
    std::cerr << "ell=" << ell*1000.0 << " ms" << std::endl;

}
Y_UDONE()

