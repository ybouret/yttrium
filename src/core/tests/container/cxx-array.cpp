#include "y/container/cxx-array.hpp"
#include "y/utest/run.hpp"
#include "y/apex/natural.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;

Y_UTEST(container_cxx_array)
{
    CxxArray<apn,Memory::Pooled> arr(5);

    std::cerr << arr << std::endl;

}
Y_UDONE()

