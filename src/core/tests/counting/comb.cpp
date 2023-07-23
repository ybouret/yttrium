
#include "y/counting/c/comb.h"
#include "y/container/cxx-array.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

Y_UTEST(counting_comb)
{
    Y_Comb ctx;

    for(size_t n=1;n<=6;++n)
    {
        for(size_t k=1;k<=n;++k)
        {
            std::cerr << "(" << n << "," << k << ")" << std::endl;
            Y_Comb_Init(&ctx,n,k);
            CxxArray<size_t, Memory::Pooled> arr(k);
            Y_Comb_Boot(&ctx,arr.legacy());
            do
            {
                std::cerr << arr << std::endl;
            } while( Y_Comb_Next(&ctx,arr.legacy()) != 0);
        }
    }

}
Y_UDONE()

