
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
    std::cerr << "lhs = " << lhs << " = " << lhs.cast<float>()       << std::endl;
    std::cerr << "rhs = " << rhs << " = " << rhs.cast<long double>() << std::endl;

    const SignType dif = Rational::Compare(lhs,rhs);
    //std::cerr << "dif = '" << Sign::ToChar(dif) << "'" << std::endl;
    Y_ASSERT( Sign::Opposite(dif) == Rational::Compare(rhs,lhs) );

    const Rational sum  = lhs + rhs;
    const Rational prod = lhs * rhs;
    Y_ASSERT(lhs == sum-rhs);
    Y_ASSERT(rhs == sum-lhs);
    if(prod!=0)
    {
        Y_ASSERT(lhs == prod/rhs);
        Y_ASSERT(rhs == prod/lhs);
    }

    if(rhs.denom==1)
    {
        const Integer &Z = rhs.numer;
        Y_ASSERT( dif == Rational::Compare(lhs,Z) );
        Y_ASSERT( Sign::Opposite(dif) == Rational::Compare(Z,lhs) );

        Y_ASSERT(sum == lhs+Z);
        Y_ASSERT(sum == Z+lhs);

        Y_ASSERT(lhs == sum-Z);
        Y_ASSERT(-lhs == Z-sum);

        Y_ASSERT(prod == lhs*Z);
        Y_ASSERT(prod == Z*lhs);

        if(rhs.numer>=0)
        {
            const Natural &N = rhs.numer.n;
            Y_ASSERT( dif == Rational::Compare(lhs,N) );
            Y_ASSERT( Sign::Opposite(dif) == Rational::Compare(N,lhs) );
            Y_ASSERT( sum == lhs + N);
            Y_ASSERT( sum == N + lhs);
            Y_ASSERT( lhs == sum-N);
            Y_ASSERT(-lhs == N-sum);
            Y_ASSERT( prod == lhs * N );
            Y_ASSERT( prod == N * lhs );
        }

        integer_t z = 0;
        if( rhs.numer.tryCast(z) )
        {
            Y_ASSERT( dif == Rational::Compare(lhs,z) );
            Y_ASSERT( Sign::Opposite(dif) == Rational::Compare(z,lhs) );

            Y_ASSERT(sum == lhs + z);
            Y_ASSERT(sum == z + lhs);
            Y_ASSERT(lhs == sum-z);
            Y_ASSERT(-lhs == z-sum);

            Y_ASSERT(prod == lhs * z);
            Y_ASSERT(prod == z * lhs);

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
    for(size_t cycle=0;cycle<1000;++cycle)
    {
        const Rational lhs(ran,ran.lt(32),1+ran.leq(30));
        const Rational rhs(ran,ran.lt(32),1+ran.leq(30));

        Y_ASSERT( __Zero__ == Rational::Compare(lhs,lhs) );
        Y_ASSERT( Rational::Compare(rhs,lhs) == Sign::Opposite( Rational::Compare(lhs,rhs) ) );

        TestPair(lhs,rhs);

    }


    std::cerr << "Incr" << std::endl;
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


    std::cerr << "Decr" << std::endl;
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

    std::cerr << "Add/Div" << std::endl;


}
Y_UDONE()
