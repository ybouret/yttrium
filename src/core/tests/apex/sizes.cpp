

#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_sizes)
{
    Y_SIZEOF(Natural);
    Y_SIZEOF(Integer);
    Y_SIZEOF(Rational);

}
Y_UDONE()
