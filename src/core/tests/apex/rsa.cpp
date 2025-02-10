
#include "y/apex/modular.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/system/wtime.hpp"


using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_rsa)
{
    Random::ParkMiller ran;
    const Natural      p   = 11;
    const Natural      q   = 17;
    const Natural      n   = p * q;
    const Natural      lam = Natural::LCM(p-1,q-1);
    //const Natural      lam = (p-1)*(q-1);
    const Natural      e   = 7;
    const Natural      d   = Modular::Inv(e,lam);

    std::cerr << "p   = " << p << std::endl;
    std::cerr << "q   = " << q << std::endl;
    std::cerr << "n   = " << n << std::endl;
    std::cerr << "lam = " << lam << std::endl;
    std::cerr << "e   = " << e << std::endl;
    std::cerr << "d   = " << d << std::endl;

    for(Natural m=0;m<n;++m)
    {
        (std::cerr << m).flush();
        const Natural c = Modular::Exp(m,e,n);
        (std::cerr << " => " << c).flush();

        const Natural r = Modular::Exp(c,d,n);

        (std::cerr << " => " << r).flush();
        Y_ASSERT(r==m);
        
        std::cerr << std::endl;
    }

}
Y_UDONE()
