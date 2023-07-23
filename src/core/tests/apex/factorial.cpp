
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_factorial)
{
    for(int i=0;i<=16;++i)
    {
        (std::cerr << std::setw(4) << i << "! = ").flush();
        const apn f = apn::Factorial(i);
        std::cerr << f;
        std::cerr << std::endl;
    }
}
Y_UDONE()
