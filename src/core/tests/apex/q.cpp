
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


    std::cerr << "Comparisons" << std::endl;
    for(size_t cycle=0;cycle<10;++cycle)
    {
        const Rational lhs(ran,ran.lt(32),1+ran.leq(30));
        const Rational rhs(ran,ran.lt(32),1+ran.leq(30));

        std::cerr << lhs << " | " << rhs << std::endl;
        Y_ASSERT( __Zero__ == Rational::Compare(lhs,lhs) );
        Y_ASSERT( Rational::Compare(rhs,lhs) == Sign::Opposite( Rational::Compare(lhs,rhs) ) );

        const Integer  quot = lhs.numer/lhs.denom;
        const SignType diff = Rational::Compare(lhs,quot);
        Y_ASSERT( Sign::Opposite(diff) == Rational::Compare(quot,lhs) );

        std::cerr << "quot = " << quot << std::endl;
        const integer_t z = quot.cast<integer_t>("quot");
        std::cerr << "z    = " << z << std::endl;
        Y_ASSERT(z==quot);
        Y_ASSERT(quot==z);
        std::cerr << "diff=" << Sign::ToChar(diff) << std::endl;
        Y_ASSERT(diff == Rational::Compare(lhs,z) );

    }



}
Y_UDONE()
