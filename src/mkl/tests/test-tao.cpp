#include "y/mkl/tao/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/concurrent/loop/simd.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"


#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename TARGET, typename SOURCE>   inline
    void Load(TARGET &target, SOURCE &source, Concurrent::SIMD &simd)
    {
        assert(target.size()<=source.size());
        struct Work
        {
            void operator()(const Concurrent::Range &range, TARGET &target, SOURCE &source)
            {
                for(size_t i=range.offset,k=range.length;k>0;--k,++i)
                {
                    target[i] = source[i];
                }
            }
        };

        Work todo = {};
        simd(todo,target,source);


        // for(size_t i=target.size();i>0;--i)             target[i] = source[i];
    }
}

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

    Vector<double> v(5,0);
    Vector<int>    u(5,0);

    Load(v,u,seq);
    



}
Y_UDONE()


