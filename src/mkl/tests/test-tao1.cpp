#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/par/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T> static inline
    void testTAO()
    {
        std::cerr << "Tao<" << RTTI::Name<T>() << ">" << std::endl;
        //Tao::Multifold<T> mf;
        //mf.make(2,10);



        std::cerr << std::endl;
    }
}

Y_UTEST(tao1)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;



    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Engine seq(seqLoop);
    Tao::Engine par(parLoop);

    {
        Vector<double> target(8,0);
        Vector<int>    source(10,0);
        for(size_t i=1;i<=source.size();++i) source[i] = int(i);

        std::cerr << "target=" << target << std::endl;
        std::cerr << "source=" << source << std::endl;
        Tao::Load(target,source);
        std::cerr << "target=" << target << std::endl;
        target.ld(0);
        std::cerr << "target=" << target << std::endl;
        Tao::Load(target,source,seq);
        std::cerr << "target=" << target << std::endl;
        target.ld(0);
        std::cerr << "target=" << target << std::endl;
        Tao::Load(target,source,par);
        std::cerr << "target=" << target << std::endl;


        testTAO<float>();
        testTAO<double>();
        testTAO<long double>();
        testTAO< XReal<float>  >();
        testTAO< XReal<double> >();
        testTAO< XReal<long double> >();
        testTAO< apq >();
    }


    {
        const size_t n=10;
        Vector<int>  target(n,0);
        Vector<int>  seqtgt(n,0);
        Vector<int>  partgt(n,0);
        Vector<int>  source(n,0);
        Vector<int>  vector(n,0);

        for(size_t i=1;i<=n;++i) target[i] = seqtgt[i] = partgt[i] = source[i] = vector[i] = int(i);

        Tao::Add(target,source);     std::cerr << target << std::endl;
        Tao::Add(seqtgt,source,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
        Tao::Add(partgt,source,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);

        {
            const int factor = 3;
            Tao::Add(target,factor,source);     std::cerr << target << std::endl;
            Tao::Add(seqtgt,factor,source,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
            Tao::Add(partgt,factor,source,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);
        }


        {
            const int factor = 7;
            Tao::Add(target,source,factor,vector);     std::cerr << target << std::endl;
            Tao::Add(seqtgt,source,factor,vector,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
            Tao::Add(partgt,source,factor,vector,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);
        }

        Tao::Sub(target,source);     std::cerr << target << std::endl;
        Tao::Sub(seqtgt,source,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
        Tao::Sub(partgt,source,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);

        {
            const int factor = 3;
            Tao::Sub(target,factor,source);     std::cerr << target << std::endl;
            Tao::Sub(seqtgt,factor,source,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
            Tao::Sub(partgt,factor,source,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);
        }


        {
            const int factor = 7;
            Tao::Sub(target,source,factor,vector);     std::cerr << target << std::endl;
            Tao::Sub(seqtgt,source,factor,vector,seq); std::cerr << seqtgt << std::endl; Y_CHECK(seqtgt==target);
            Tao::Sub(partgt,source,factor,vector,par); std::cerr << partgt << std::endl; Y_CHECK(partgt==target);
        }



    }




}
Y_UDONE()


