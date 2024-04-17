
#include "y/chemical/lang/rosary.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(rosary)
{
    Rosary & rosary = Rosary::Instance();
    std::cerr << rosary.callSign() << std::endl;
}
Y_UDONE()

