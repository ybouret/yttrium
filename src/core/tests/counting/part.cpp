
#include "y/counting/c/part.h"
#include "y/container/cxx-array.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

Y_UTEST(counting_part)
{

    Y_Part part;
    for(size_t n=1;n<=6;++n)
    {
        std::cerr << "n=" << n << std::endl;
        CxxArray<size_t,Memory::Pooled> arr(n);
        Y_Part_Init(&part,n);
        Y_Part_Boot(&part,arr.legacy());
        do
        {
            Core::Display(std::cerr << "\t", &arr[1], part.k) << std::endl;
        }
        while( 0 != Y_Part_Next(&part,arr.legacy()) );

    }

}
Y_UDONE()

