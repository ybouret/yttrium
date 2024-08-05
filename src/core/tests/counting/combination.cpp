
#include "y/counting/combination.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(counting_combination)
{
    if(argc>2)
    {
        const size_t n = ASCII::Convert::To<size_t>(argv[1],"n");
        const size_t k = ASCII::Convert::To<size_t>(argv[2],"k");
        const apn    r = Kemp::Natural::Comb(n,k);
        std::cerr << "(" << n << "," << k << ")=" << r << std::endl;
    }
    else
    {
        for(size_t n=1;n<=8;++n)
        {
            std::cerr << "n=" << n << std::endl;
            for(size_t k=1;k<=n;++k)
            {
                Combination comb(n,k);
                size_t      count = 0;
                do
                {
                    std::cerr << comb << std::endl;
                    ++count;
                } while( comb.next() );
                Y_ASSERT(count==comb.total);
                comb.boot();

                count = 0;
                do
                {
                    ++count;
                } while( comb.next() );
                Y_ASSERT(count==comb.total);
            }
        }
    }

}
Y_UDONE()

