

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
        for(natural_t n=0;n<=22;++n)
        {
            const Natural f = Natural::Factorial(n);
            std::cerr << std::setw(4) << n << "! = " << f;

            if(f->bits>32)
            {
                std::cerr << "*";
            }

            if(f->bits>64)
            {
                std::cerr << "*";
            }
            std::cerr << std::endl;
        }
    }

}
Y_UDONE()
