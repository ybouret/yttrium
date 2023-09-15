
#include "y/ordered/orderly.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparison.hpp"
#include "y/random/shuffle.hpp"
#include "y/container/cxx/array.hpp"

using namespace Yttrium;

Y_UTEST(ordered_locate)
{
    Random::Rand ran;

    CxxArray<int,Memory::Pooled> data(10 + ran.leq(22));
    for(size_t i=data.size();i>0;--i) data[i] = int(i);
    std::cerr << "data=" << data << std::endl;

    int    arr[100];
    size_t num = 0;

    for(size_t iter=0;iter<10;++iter)
    {
        Random::Shuffle::Range(data.begin(), data.size(), ran);
        for(size_t i=1;i<=data.size();++i)
        {
            size_t ipos=0;
            Y_ASSERT(false==Orderly::Locate(ipos,data[i],arr,num, Comparison::CxxIncreasing<int>));
            Orderly::Insert(ipos, data[i], arr, num);
            Core::Display(std::cerr,arr,num) << std::endl;
        }
        Orderly::Finish(arr,num);
    }



}
Y_UDONE()
