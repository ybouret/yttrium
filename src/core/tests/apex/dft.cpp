#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Apex;

Y_UTEST(apex_dft)
{
    Natural a = 2;
    Natural b = 6;
    const BlockPtr ptr( Natural::DFT(a,b) );
}
Y_UDONE()

