#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_dft)
{
    Natural a = 0x0310;
    Natural b = 0x0044;
    const BlockPtr ptr( Natural::DFT(a,b) );
    std::cerr << "p=" << (a*b).toHex() << " = " << (a*b).toDec() << std::endl;
}
Y_UDONE()

