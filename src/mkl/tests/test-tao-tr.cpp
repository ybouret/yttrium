
#include "y/mkl/tao/seq/tridiag.hpp"
#include "y/mkl/tao/seq/level2.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/container/cxx/array.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    static inline void testTr( Random::Bits &ran )
    {
        typedef typename ScalarFor<T>::Type ScalarType;
        const ScalarType _0(0);

        for(size_t n=1;n<=8;++n)
        {
            MKL::TriDiag<T>            tr(n);
            CxxArray<T,Memory::Pooled> u(n);
            CxxArray<T,Memory::Pooled> v(n);
            CxxArray<T,Memory::Pooled> r(n);


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

            Tao::MultiAdd<T> xma;
            Tao::Mul(v,M,r,xma);
            std::cerr << "v =" << v << std::endl;

            Tao::TriDiagMul(u,tr,r,Type2Type<T>());
            std::cerr << "u =" << u << std::endl;

#if 1
            Tao::ComputeMod2<T> Mod2;
            const ScalarType arg = Mod2(u,v)/ScalarType(n);
            const ScalarType rms = Sqrt<ScalarType>::Of(arg);
            std::cerr << "\trms=" << rms << std::endl;
            Y_CHECK(_0 == rms);
#endif

        }
        std::cerr << std::endl;
    }
}

Y_UTEST(taoTr)
{
    Random::Rand ran;

    testTr<float>(ran);
    testTr<double>(ran);
    testTr<long double>(ran);

    testTr< XReal<float>       >(ran);
    testTr< XReal<double>      >(ran);
    testTr< XReal<long double> >(ran);

    testTr< Complex<float> >(ran);
    testTr< Complex<double> >(ran);
    testTr< Complex<long double> >(ran);

}
Y_UDONE()
