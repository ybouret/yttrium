

#include "y/counting/combination.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(counting_combination)
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
Y_UDONE()

