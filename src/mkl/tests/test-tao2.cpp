#include "y/mkl/tao/level2.hpp"

#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

#include "../../core/tests/main.hpp"

using namespace Yttrium;
using namespace MKL;

Y_UTEST(tao2)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;



    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Engine seq(seqLoop);
    Tao::Engine par(parLoop);
    Random::Rand ran;

    {
        const size_t nr = 4;
        const size_t nc = 8;

        Vector<int> target(nr,0);
        Vector<int> seqtgt(nr,0);
        Vector<int> partgt(nr,0);
        Matrix<int> M(nr,nc);
        Vector<int> source(nc,0);



        for(size_t j=1;j<=nc;++j)
        {
            source[j] = ran.in<int>(-10,10);
            for(size_t i=1;i<=nr;++i)
                M[i][j] = ran.in<int>(-10,10);
        }

        std::cerr << "source=" << source << std::endl;
        std::cerr << "M     =" << M << std::endl;

        Antelope::Caddy<int> xma;
        Tao::Mul(target,M,source,xma);
        std::cerr << "target=" << target << std::endl;

        Tao::Mul(seqtgt,M,source,xma,seq);
        std::cerr << "seqtgt=" << seqtgt << std::endl;
        Y_CHECK(seqtgt==target);

        Tao::Mul(partgt,M,source,xma,par);
        std::cerr << "partgt=" << partgt << std::endl;
        Y_CHECK(partgt==target);
    }

}
Y_UDONE()

