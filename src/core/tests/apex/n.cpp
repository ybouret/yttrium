
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_n)
{
    Natural n = 1025;

    std::cerr << n << std::endl;
    n = 111;
    std::cerr << n << std::endl;


    Y_SIZEOF(Natural);

}
Y_UDONE()
