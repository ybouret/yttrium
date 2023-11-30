#include "y/mkl/tao/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/simd.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace Yttrium
{
    namespace MKL
    {



    }

}

Y_UTEST(tao)
{
    const Concurrent::Topology topo;
    Concurrent::SIMD par( new Concurrent::Crew(topo) );

}
Y_UDONE()


