#include "y/mkl/tao/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;



Y_UTEST(tao)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Concurrent::SIMD par( new Concurrent::Crew(topo) );
    Concurrent::SIMD seq( new Concurrent::Mono()     );

    size_t n = 4;
    seq.dispatch(n);
    par.dispatch(n);

    std::cerr << std::endl;
    seq();

    std::cerr << std::endl;
    par();

    std::cerr << std::endl;


}
Y_UDONE()


