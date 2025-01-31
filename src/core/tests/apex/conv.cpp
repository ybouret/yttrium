
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_conv)
{
    {
        Natural num = 1177;
        Natural den = 110;
        const float f = Natural::Ratio<float>(num,den);
        std::cerr << f << std::endl;
    }
}
Y_UDONE()
