#include "y/mkl/tao/level1.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"
#include "y/sequence/vector.hpp"
#include "y/system/rtti.hpp"

#include "y/utest/run.hpp"
#include "../../core/tests/main.hpp"

#include "y/type/conversion.hpp"

using namespace Yttrium;
using namespace MKL;




namespace
{
    template <typename T> static inline
    void testTAO(Random::Bits &ran,
                 Tao::Engine  &seq,
                 Tao::Engine  &par)
    {
        std::cerr << "Tao<" << RTTI::Name<T>() << ">" << std::endl;

        Vector<T> target(8,0);
        Vector<T> source(10,0);
        for(size_t i=1;i<=source.size();++i) source[i] = Bring<T>::Get(ran);

        std::cerr << "target=" << target << std::endl;
        std::cerr << "source=" << source << std::endl;
        Tao::Load(target,source);
        std::cerr << "target=" << target << std::endl;
        target.ld(0);
        //std::cerr << "target=" << target << std::endl;
        Tao::Load(target,source,seq);
        std::cerr << "target=" << target << std::endl;
        target.ld(0);
        //std::cerr << "target=" << target << std::endl;
        Tao::Load(target,source,par);
        std::cerr << "target=" << target << std::endl;

        std::cerr << std::endl;
    }


}

Y_UTEST(tao1)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Random::Rand               ran;


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


        Tao::MultiAdd<int> xmf;
        const int rawDot = Tao::DotProduct<int>::Of(source,vector,xmf);
        std::cerr << "rawDot=" << rawDot << std::endl;

        {
            Vector<apq>        lhs;
            Vector<apq>        rhs;
            Tao::MultiAdd<apq> amf;
            for(size_t i=5+ran.leq(10);i>0;--i)
            {
                lhs << apq(10,10,ran);
                rhs << apq(10,10,ran);
            }
            std::cerr << "lhs=" << lhs << std::endl;
            std::cerr << "rhs=" << rhs << std::endl;
            const apq aDot = Tao::DotProduct<apq>::Of(lhs,rhs,amf);
            std::cerr << "dot=" << aDot << std::endl;
        }


    }

    {
        Vector<double>                 lhs;
        Vector<float>                  rhs;
        Tao::MultiAdd< XReal<double> > xmf;

        for(size_t i=5+ran.leq(10);i>0;--i)
        {
            lhs << ran.symm<double>();
            rhs << ran.symm<float>();
        }
        std::cerr << "lhs=" << lhs << std::endl;
        std::cerr << "rhs=" << rhs << std::endl;
        const double dot = double(Tao::DotProduct< XReal<double> >::Of(lhs,rhs,xmf));
        std::cerr << "dot=" << dot << std::endl;
    }

    {
        Vector< Complex<float> > v;
        for(size_t i=5+ran.leq(10);i>0;--i)
        {
            const Complex<float> z( ran.symm<float>(), ran.symm<float>() );
            v << z;
        }
        std::cerr << "v=" << v << std::endl;
        Tao::MultiAdd<double> xm;
        const double n2 = Tao::Cog::SquaredNorm<double>::Of(v,xm);
        std::cerr << "n2=" << n2 << std::endl;

    }

#define ARGS ran,seq,par
    testTAO<float>(ARGS);
    testTAO<double>(ARGS);
    testTAO<long double>(ARGS);
    testTAO< XReal<float>  >(ARGS);
    testTAO< XReal<double> >(ARGS);
    testTAO< XReal<long double> >(ARGS);
    testTAO< apq >(ARGS);

}
Y_UDONE()


