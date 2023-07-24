#include "y/container/cxx-array.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

template <typename CLASS>
void displayAll(CLASS &cls)
{
    for(typename CLASS::Iterator it=cls.begin();it!=cls.end();++it)
    {
        std::cerr << ' ' << *it;
    }
    std::cerr << std::endl;

    const CLASS &cst = cls;
    for(typename CLASS::ConstIterator it=cst.begin();it!=cst.end();++it)
    {
        std::cerr << ' ' << *it;
    }
    std::cerr << std::endl;
}


Y_UTEST(container_cxx_array)
{
    CxxArray<apn,Memory::Pooled> arr(5);

    arr[1] = 2;
    arr[2] = 7;
    arr[5] = 17;
    
    std::cerr << arr << std::endl;

    displayAll(arr);


}
Y_UDONE()

