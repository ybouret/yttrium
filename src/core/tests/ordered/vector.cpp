#include "y/ordered/vector.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/container/cxx-array.hpp"

using namespace Yttrium;


Y_UTEST(ordered_vector)
{
    { OrderedVector<int> V0; }

    OrderedVector<int> V(5);

    Random::Rand ran;

    CxxArray<int,Memory::Pooled> data(10 + ran.leq(22));
    for(size_t i=data.size();i>0;--i) data[i] = int(i);
    std::cerr << "data=" << data << std::endl;

    V.free();
    {
        for(size_t i=data.size();i>0;--i)
        {
            Y_ASSERT( V.insert( data[i] ) );
        }
        std::cerr << V << std::endl;
    }


}
Y_UDONE()

