
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



}
Y_UDONE()

