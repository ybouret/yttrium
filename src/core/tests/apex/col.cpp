
#include "y/apex/api/colinearity.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_col)
{
    Random::ParkMiller ran;

    Vector<int> a,b;
    a << 0;
    b << 0;
    Y_CHECK(Apex::Colinearity::Of(a,b));
    a << 1;
    b << -2;
    Y_CHECK(Apex::Colinearity::Of(a,b));


}
Y_UDONE()
