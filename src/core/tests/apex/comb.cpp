
#include "y/apex/natural.hpp"
#include "y/utest/run.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;
using namespace Apex;



Y_UTEST(apex_comb)
{

    for(natural_t n=1;n<=10;++n)
    {
        std::cerr << std::setw(2) << n << ":";
        for(size_t k=0;k<=n;++k)
        {
            const Natural c = Natural::Comb(n,k);
            std::cerr << ' ' << c;
        }
        std::cerr << std::endl;
    }

}
Y_UDONE()
