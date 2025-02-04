#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_dft)
{

    Natural a = 0x0310;
    Natural b = 0x0044;
    if(argc>2)
    {
        a = argv[1];
        b = argv[2];
    }
    const Natural mul = a*b;
    const Natural dft = Natural::FFT(a,b);
    std::cerr << "mul=" << mul.toHex() << " / dft=" << dft.toHex() << std::endl;
}
Y_UDONE()

