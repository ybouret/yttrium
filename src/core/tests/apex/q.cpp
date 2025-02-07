
#include "y/apex/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/stream/libc/input.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;



static inline void TestPair(const Rational &lhs, const Rational &rhs)
{
    std::cerr << "lhs=" << lhs << " = " << lhs.cast<float>()       << std::endl;
    std::cerr << "rhs=" << rhs << " = " << rhs.cast<long double>() << std::endl;

    const SignType dif = Rational::Compare(lhs,rhs);
    std::cerr << "dif='" << Sign::ToChar(dif) << "'" << std::endl;

    if(rhs.denom==1)
    {
        Y_CHECK( dif == Rational::Compare(lhs,rhs.numer) );
        Y_CHECK( Sign::Opposite(dif) == Rational::Compare(rhs.numer,lhs) );

        if(rhs.numer>=0)
        {
            Y_CHECK( dif == Rational::Compare(lhs,rhs.numer.n) );
            Y_CHECK( Sign::Opposite(dif) == Rational::Compare(rhs.numer.n,lhs) );

        }
        integer_t z = 0;
        if( rhs.numer.tryCast(z) )
        {
            Y_CHECK( dif == Rational::Compare(lhs,z) );
            Y_CHECK( Sign::Opposite(dif) == Rational::Compare(z,lhs) );
        }

    }
}

Y_UTEST(apex_q)
{
    Random::ParkMiller ran;

    if(argc>2)
    {
        const Rational lhs = String(argv[1]);
        const Rational rhs = String(argv[2]);
        TestPair(lhs,rhs);
        TestPair(rhs,lhs);
        return 0;
    }


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
    for(size_t cycle=0;cycle<100;++cycle)
    {
        const Rational lhs(ran,ran.lt(32),1+ran.leq(30));
        const Rational rhs(ran,ran.lt(32),1+ran.leq(30));

        std::cerr << lhs << " | " << rhs << std::endl;
        Y_ASSERT( __Zero__ == Rational::Compare(lhs,lhs) );
        Y_ASSERT( Rational::Compare(rhs,lhs) == Sign::Opposite( Rational::Compare(lhs,rhs) ) );

        const Integer  quot = lhs.numer/lhs.denom;
        const SignType diff = Rational::Compare(lhs,quot);
        Y_ASSERT( Sign::Opposite(diff) == Rational::Compare(quot,lhs) );

        const integer_t z = quot.cast<integer_t>("quot");
        Y_ASSERT(z==quot);
        Y_ASSERT(quot==z);
        Y_ASSERT(diff == Rational::Compare(lhs,z) );
        Y_ASSERT(Sign::Opposite(diff) == Rational::Compare(z,lhs) );

        const Natural n = quot.n;
        const SignType naturalDiff = Rational::Compare(lhs,n);
        Y_ASSERT( Sign::Opposite(naturalDiff) == Rational::Compare(n,lhs) );

    }


    std::cerr << "Additions" << std::endl;

    for(Rational q(-8,3); q <= 2; ++q)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;

    for(Rational q(-8,3); q <= 2; q++)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;


    for(Rational q=-2; q <= 2; ++q)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;

    for(Rational q=-2; q <= 2; q++)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;


    std::cerr << "Subtractions" << std::endl;
    for(Rational q(8,3); q >= -2; --q)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;
    for(Rational q(8,3); q >= -2; q--)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;

    for(Rational q=2; q >= -2; --q)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;

    for(Rational q=2; q >= -2; q--)
    {
        std::cerr << " " << q;
    }
    std::cerr << std::endl;

}
Y_UDONE()
