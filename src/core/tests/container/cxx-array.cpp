#include "y/container/cxx/array.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/container/iterator/display-all.hpp"
#include "y/sequence/vector.hpp"

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

    Vector<size_t> iv;
    for(size_t i=10;i>0;--i) iv.pushTail(i);
    std::cerr << "iv =" << iv << std::endl;
    IArray vrr(CopyOf,iv);
    std::cerr << "vrr=" << vrr << std::endl;

    {
        IArray A(5);
        IArray B(0);
        std::cerr << "A=" << A << std::endl;
        std::cerr << "B=" << B << std::endl;
        A.swapWith(B);
        std::cerr << "A=" << A << std::endl;
        std::cerr << "B=" << B << std::endl;
    }


}
Y_UDONE()

