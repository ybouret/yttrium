

#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_factorial)
{
    if(argc>1)
    {
        const natural_t n = ASCII::Convert::To<natural_t>(argv[1],"n");
        std::cerr << n << "! = " << Natural::Factorial(n) << std::endl;
    }
    else
    {
        for(natural_t n=0;n<=20;++n)
        {
            std::cerr << std::setw(4) << n << "! = " << Natural::Factorial(n) << std::endl;
        }
    }

}
Y_UDONE()
