#include "y/container/cxx/array.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;



Y_UTEST(container_cxx_array)
{
    typedef CxxArray<apn,Memory::Pooled> IArray;
    IArray arr(5);
    Y_CHECK(5==arr.size());

    arr[1] = 2;
    arr[2] = 7;
    arr[5] = 17;
    
    std::cerr << arr << std::endl;

    std::cerr << "arr:" << std::endl;
    Iterating::DisplayAll::Of(arr);

    {
        IArray brr(arr);
        std::cerr << "brr:" << std::endl;
        Iterating::DisplayAll::Of(brr);
        Y_CHECK(arr==brr);
    }


}
Y_UDONE()

