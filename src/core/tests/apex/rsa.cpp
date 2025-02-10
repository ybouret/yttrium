
#include "y/apex/api/modular.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/wtime.hpp"


using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_rsa)
{
    Random::ParkMiller ran;
    const Natural      p   = 17;
    const Natural      q   = 67;
    const Natural      n   = p * q;
    const Natural      lam = Natural::LCM(p-1,q-1);
    const Natural      e   = 7;
    const Natural      d   = Modular::Inv(e,lam);

    std::cerr << "p   = " << std::setw(20) << p << std::endl;
    std::cerr << "q   = " << std::setw(20) << q << std::endl;
    std::cerr << "n   = " << std::setw(20) << n << std::endl;
    std::cerr << "lam = " << std::setw(20) << lam << std::endl;
    std::cerr << "e   = " << std::setw(20) << e << std::endl;
    std::cerr << "d   = " << std::setw(20) << d << std::endl;

    std::cerr << '[';
    for(Natural m=0;m<n;++m)
    {
        const Natural c = Modular::Exp(m,e,n);
        const Natural r = Modular::Exp(c,d,n);
        Y_ASSERT(r==m);
        (std::cerr << '#').flush();
    }
    std::cerr << ']' << std::endl;

}
Y_UDONE()
