
#include "y/apex/integer.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_conv)
{
    Natural num = 6;
    Natural den = 7;
    if(argc>2)
    {
        num = argv[1];
        den = argv[2];
    }

    std::cerr << num << "/" << den;
    std::cerr << " = " << Natural::Ratio<float>(num,den);
    std::cerr << " = " << Natural::Ratio<double>(num,den);
    std::cerr << " = " << Natural::Ratio<long double>(num,den);
    std::cerr << std::endl;

}
Y_UDONE()
