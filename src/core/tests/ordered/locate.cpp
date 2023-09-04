
#include "y/ordered/locate.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparison.hpp"
#include "y/random/shuffle.hpp"
#include "y/container/cxx-array.hpp"

using namespace Yttrium;

Y_UTEST(ordered_locate)
{
    Random::Rand ran;

    CxxArray<int,Memory::Pooled> data(10 + ran.leq(10));
    for(size_t i=data.size();i>0;--i) data[i] = int(i);
    Random::Shuffle::Range(data.begin(), data.size(), ran);
    std::cerr << "data=" << data << std::endl;

    Vector<int> ivec( data.size(), AsCapacity);
    for(size_t i=1;i<=data.size();++i)
    {
        size_t p = 0;
        Y_CHECK(false==Core::Locate(p,data[i],ivec,Comparison::CxxIncreasing<int>));
        std::cerr << "@" << p << std::endl;
        break;
    }

}
Y_UDONE()
