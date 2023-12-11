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
        Tao::Multifold<T> mf;
        mf.make(2,10);


        std::cerr << std::endl;
    }
}

Y_UTEST(tao)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;



    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Carver seq(seqLoop);
    Tao::Carver par(parLoop);

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


    Y_SIZEOF(Tao::DynamicAdd<float>);
    Y_SIZEOF(Tao::DynamicAdd<double>);
    Y_SIZEOF(Tao::DynamicAdd<long double>);
    Y_SIZEOF(Tao::DynamicAdd< XReal<float> >);
    Y_SIZEOF(Tao::DynamicAdd< XReal<double> >);
    Y_SIZEOF(Tao::DynamicAdd< XReal<long double> >);

    Y_SIZEOF(Tao::DynamicAdd< Complex<float> >);
    Y_SIZEOF(Tao::DynamicAdd< Complex<double> >);
    Y_SIZEOF(Tao::DynamicAdd< Complex<long double> >);
    Y_SIZEOF(Tao::DynamicAdd< Complex< XReal<float> > >);
    Y_SIZEOF(Tao::DynamicAdd< Complex< XReal<double> > >);
    Y_SIZEOF(Tao::DynamicAdd< Complex< XReal<long double> > >);

    Y_SIZEOF( Tao::DynamicAdd< apn > );
    Y_SIZEOF( Tao::DynamicAdd< apz > );
    Y_SIZEOF( Tao::DynamicAdd< apq > );

    testTAO<float>();
    testTAO<double>();
    testTAO<long double>();
    testTAO< XReal<float>  >();
    testTAO< XReal<double> >();
    testTAO< XReal<long double> >();
    testTAO< apq >();
}
Y_UDONE()


