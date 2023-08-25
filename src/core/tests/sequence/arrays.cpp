#include "y/sequence/arrays.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "../main.hpp"

using namespace Yttrium;

Y_UTEST(sequence_arrays)
{
    Random::Rand ran;
    Y_SIZEOF(LightArray<double>);
    Y_SIZEOF(LightArray<String>);

#if 0
    Arrays<double,Memory::Pooled> arr(5,10);

    for(size_t i=1;i<=5;++i)
    {
        FillWritable(arr[i],ran);
        std::cerr << arr[i] << std::endl;
    }

    for(size_t i=1;i<=5;++i)
    {
        std::cerr << arr[i] << std::endl;
    }
#endif

}
Y_UDONE()

