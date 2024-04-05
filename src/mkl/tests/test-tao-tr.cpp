
#include "y/mkl/tao/par/tridiag.hpp"
#include "y/mkl/tao/seq/tridiag.hpp"
#include "y/mkl/tao/seq/level2.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/pooled.hpp"

#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"

#include "y/concurrent/thread.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    static inline void testTr(Random::Bits &ran,
                              Tao::Driver  &seq,
                              Tao::Driver  &par)
    {
        typedef typename ScalarFor<T>::Type ScalarType;
        //const ScalarType _0(0);

        for(size_t n=1;n<=8;++n)
        {
            MKL::TriDiag<T>            tr(n);
            CxxArray<T,Memory::Pooled> u(n);
            CxxArray<T,Memory::Pooled> v(n);
            CxxArray<T,Memory::Pooled> r(n);
            CxxArray<T,Memory::Pooled> w(n);
            CxxArray<T,Memory::Pooled> x(n);


            for(size_t i=1;i<=n;++i)
            {
                tr.a[i] = ran.symm<float>();
                tr.b[i] = ran.symm<float>();
                tr.c[i] = ran.symm<float>();
                r[i]    = ran.symm<float>();
            }

            Matrix<T> M(n,n);
            tr.sendTo(M);
            std::cerr << "tr= " << tr << std::endl;
            std::cerr << "M = " << M  << std::endl;
            std::cerr << "r = " << r  << std::endl;

            Antelope::Caddy<T> xma;
            Tao::Mul(v,M,r,xma);
            std::cerr << "v = " << v << std::endl;

            Tao::TriDiagMul(u,tr,r,Type2Type<T>());
            std::cerr << "u = " << u << std::endl;

            Tao::TriDiagMul(seq,w,tr,r,Type2Type<T>());
            std::cerr << "w = " << w << std::endl;

            Tao::TriDiagMul(par,x,tr,r,Type2Type<T>());
            std::cerr << "x = " << x << std::endl;

#if 1
            Tao::ComputeMod2<T> Mod2;
            {
                const ScalarType arg = Mod2(u,v)/ScalarType(n);
                const ScalarType rms = Sqrt<ScalarType>::Of(arg);
                std::cerr << "\trms_mat=" << rms << std::endl;
            }
            //Y_CHECK(_0 == rms);
#endif


            {
                const ScalarType arg = Mod2(u,w)/ScalarType(n);
                const ScalarType rms_seq = Sqrt<ScalarType>::Of(arg);
                std::cerr << "\trms_seq=" << rms_seq << std::endl;
                //Y_CHECK(_0 == rms_seq);
            }

            {
                const ScalarType arg = Mod2(u,x)/ScalarType(n);
                const ScalarType rms_par = Sqrt<ScalarType>::Of(arg);
                std::cerr << "\trms_par=" << rms_par << std::endl;
                //Y_CHECK(_0 == rms_par);
            }


        }
        std::cerr << std::endl;
    }
}

Y_UTEST(taoTr)
{
    Concurrent::Thread::Verbose = Environment::Flag("VERBOSE");
    const Concurrent::Topology topo;
    Random::Rand               ran;


    Concurrent::SharedLoop seqLoop = new Concurrent::Mono();
    Concurrent::SharedLoop parLoop = new Concurrent::Crew(topo);

    Tao::Driver seq(seqLoop);
    Tao::Driver par(parLoop);

#define ARGS ran,seq,par

    testTr<float>(ARGS);
    testTr<double>(ARGS);
    testTr<long double>(ARGS);

    testTr< XReal<float>       >(ARGS);
    testTr< XReal<double>      >(ARGS);
    testTr< XReal<long double> >(ARGS);

    testTr< Complex<float> >(ARGS);
    testTr< Complex<double> >(ARGS);
    testTr< Complex<long double> >(ARGS);

}
Y_UDONE()
