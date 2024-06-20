#include "y/kemp/modular.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(kemp_modular)
{
#if 0
    const apn p = 3;
    const apn q = 11;
#endif

    const apn p = 67;
    const apn q = 107;
    const apn n = p * q;
    const apn phi = (p-1)*(q-1);
    const apn e   = 7;
    const apn d   = Kemp::Modular::Inv(e,phi);

    std::cerr << "p   = " << std::setw(20) << p   << std::endl;
    std::cerr << "q   = " << std::setw(20) << q   << std::endl;
    std::cerr << "n   = " << std::setw(20) << n   << std::endl;
    std::cerr << "phi = " << std::setw(20) << phi << std::endl;
    std::cerr << "e   = " << std::setw(20) << e   << std::endl;
    std::cerr << "d   = " << std::setw(20) << d   << std::endl;

    std::cerr << '[';
    for(apn M=0;M<n;++M)
    {
        const apn C = Kemp::Modular::Exp(M,e,n);
        const apn D = Kemp::Modular::Exp(C,d,n);
        //std::cerr << "M=" << M << " => C=" << C << " => " << D << std::endl;
        Y_ASSERT(D==M);
        (std::cerr << '#').flush();
    }
    std::cerr << ']' << std::endl;

}
Y_UDONE()
