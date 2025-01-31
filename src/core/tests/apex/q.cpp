
#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_q)
{
    Random::ParkMiller ran;

    std::cerr << Rational(12,3) << std::endl;

    {
        const Rational half(1,2);
        const Integer  one = 1;
        const Rational _1(1);

        Y_CHECK(half==half);
        Y_CHECK(!(half!=half));

        Y_CHECK(half!=one);
        Y_CHECK(one!=half);

        Y_CHECK(1!=half);
        Y_CHECK(half!=1);

        Y_CHECK(_1==1);
        Y_CHECK(1==_1);

        Y_CHECK(!(_1!=1));
        Y_CHECK(!(1!=_1));


        Y_CHECK(_1==one);
        Y_CHECK(one==_1);

        Y_CHECK(!(_1!=one));
        Y_CHECK(!(one!=_1));
    }

    for(size_t cycle=0;cycle<10;++cycle)
    {

    }

}
Y_UDONE()
