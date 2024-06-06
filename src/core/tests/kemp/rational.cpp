
#include "y/kemp/rational.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_rational)
{
    Random::ParkMiller ran;

    {
        apq q; std::cerr << q << std::endl;

        q = apq(6,3);
        std::cerr << q << std::endl;

        q = apq(7,13); q *= apq(-4,5); std::cerr << q << std::endl;
        q = apq(7,13); q /= apq(5,4);   std::cerr << q << std::endl;

        q = apq(7,13); apz z = -3; q/=z; std::cerr << q << std::endl;

        q = apq(7,13); apn n =  7; q/=n; std::cerr << q << std::endl;
        q = apq(7,13);  q/=8;            std::cerr << q << std::endl;

        q = apq(7,13);
        z = -3;
        std::cerr << z << "/(" << q << ") = " << z/q << std::endl;

        n = 3;
        std::cerr << n << "/(" << q << ") = " << n/q << std::endl;

        std::cerr << 4 << "/(" << q << ") = " << 4/q << std::endl;

        q = apq(7,13);
        apq one(1);
        z = 1;
        n = 1;
        std::cerr << q + one <<  " | " << one+q << std::endl;
        std::cerr << q + z   <<  " | " <<  z+q << std::endl;
        std::cerr << q + n   <<  " | " <<  n+q << std::endl;
        std::cerr << q + 1   <<  " | " <<  1+q << std::endl;

        std::cerr << ++q << std::endl;
        std::cerr << q++ << std::endl;
        std::cerr << q   << std::endl;

        q = apq(7,13);
        std::cerr << q - one <<  " | " << one-q << std::endl;
        std::cerr << q - z   <<  " | " <<  z-q << std::endl;
        std::cerr << q - n   <<  " | " <<  n-q << std::endl;
        std::cerr << q - 1   <<  " | " <<  1-q << std::endl;

        std::cerr << --q << std::endl;
        std::cerr << q-- << std::endl;
        std::cerr << q   << std::endl;
    }

    {
        const apq     oneR(1);
        const apn     oneN(1);
        const apz     oneZ(1);
        const int64_t oneU(1);

        apq p(1,2);
        apq q(4,3);
        std::cerr << "compare(" << p << ":" << q << ")" << std::endl;
        std::cerr << apq::Compare(p,q) << std::endl;
        std::cerr << apq::Compare(q,p) << std::endl;
        std::cerr << apq::Compare(p,p) << std::endl;
        std::cerr << "compare(" << p << ":" << oneU << ")" << std::endl;
        std::cerr << apq::Compare(p,oneR) << std::endl;
        std::cerr << apq::Compare(p,oneN) << std::endl;
        std::cerr << apq::Compare(p,oneZ) << std::endl;
        std::cerr << apq::Compare(p,oneU) << std::endl;
        std::cerr << "compare(" << q << ":" << oneU << ")" << std::endl;
        std::cerr << apq::Compare(q,oneR) << std::endl;
        std::cerr << apq::Compare(q,oneN) << std::endl;
        std::cerr << apq::Compare(q,oneZ) << std::endl;
        std::cerr << apq::Compare(q,oneU) << std::endl;

    }

    std::cerr << "fwd" << std::endl;
    for(apq i(-1,2); i <= 3; ++i)
    {
        std::cerr << i << " => " << i.sqr() << std::endl;
    }

    std::cerr << "rev" << std::endl;
    for(apq i(5,2);  i > -7 ; i--)
    {
        std::cerr << i << " => " << i.sqr() << std::endl;
    }

    {
        const apq     oneQ(1);
        const int64_t oneI(1);
        const apz     oneZ(1);
        const apn     oneN(1);

        const apq     negQ(1);
        const int64_t negI(1);
        const apz     negZ(1);
        const apn     negN(1);

        for(size_t k=0;k<16;++k)
        {
            const apq l(10,10,ran);
            const apq r(10,10,ran);

            std::cerr << "l=" << l << ", r=" << r << std::endl;
            Y_ASSERT( Rational::Compare(l,r) == Sign::Opposite( Rational::Compare(r,l) ) );

            {
                const SignType cmp = Rational::Compare(l,oneQ);
                Y_ASSERT( cmp == Rational::Compare(l,oneZ) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(oneZ,l) ) );
                Y_ASSERT( cmp == Rational::Compare(l,oneN) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(oneN,l) ) );
                Y_ASSERT( cmp == Rational::Compare(l,oneI) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(oneI,l) ) );
            }

            {
                const SignType cmp = Rational::Compare(l,negQ);
                Y_ASSERT( cmp == Rational::Compare(l,negZ) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(negZ,l) ) );
                Y_ASSERT( cmp == Rational::Compare(l,negN) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(negN,l) ) );
                Y_ASSERT( cmp == Rational::Compare(l,negI) );
                Y_ASSERT( cmp == Sign::Opposite( Rational::Compare(negI,l) ) );
            }

            {
                const apq sum = l+oneQ;
                Y_ASSERT( sum == l+oneZ);
                Y_ASSERT( sum == oneZ+l);
                Y_ASSERT( sum == l+oneN);
                Y_ASSERT( sum == oneN+l);
                Y_ASSERT( sum == l+oneI);
                Y_ASSERT( sum == oneI+l);
            }

            {
                const apq sum = l+negQ;
                Y_ASSERT( sum == l+negZ);
                Y_ASSERT( sum == negZ+l);
                Y_ASSERT( sum == l+negN);
                Y_ASSERT( sum == negN+l);
                Y_ASSERT( sum == l+negI);
                Y_ASSERT( sum == negI+l);
            }

            {
                const apq d1 = l-oneQ;
                const apq d2 = oneQ-l;
                Y_ASSERT(d1 == l-oneZ);
                Y_ASSERT(d1 == l-oneN);
                Y_ASSERT(d1 == l-oneI);
                Y_ASSERT(d2 == oneZ-l);
                Y_ASSERT(d2 == oneN-l);
                Y_ASSERT(d2 == oneI-l);
            }

            {
                const apq d1 = l-negQ;
                const apq d2 = negQ-l;
                Y_ASSERT(d1 == l-negZ);
                Y_ASSERT(d1 == l-negN);
                Y_ASSERT(d1 == l-negI);
                Y_ASSERT(d2 == negZ-l);
                Y_ASSERT(d2 == negN-l);
                Y_ASSERT(d2 == negI-l);
            }
        }
    }



}
Y_UDONE()

