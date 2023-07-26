
#include "y/counting/c/part.h"
#include "y/container/cxx-array.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

#include <cstring>

using namespace Yttrium;

Y_UTEST(counting_part)
{

    size_t counts[32];

    memset(counts,0,sizeof(counts));
    Y_Part part;
    for(size_t n=1;n<=16;++n)
    {
        std::cerr << "n=" << n << std::endl;
        CxxArray<size_t,Memory::Pooled> arr(n);
        Y_Part_Init(&part,n);
        Y_Part_Boot(&part,arr.legacy());
        size_t count = 0;
        do
        {
            ++count;
            Core::Display(std::cerr << "\t", &arr[1], part.k) << std::endl;
        }
        while( 0 != Y_Part_Next(&part,arr.legacy()) );
        counts[n] = count;
    }
    for(size_t n=1;n<=16;++n)
    {
        std::cerr << n << " => " << counts[n] << std::endl;
    }

}
Y_UDONE()

