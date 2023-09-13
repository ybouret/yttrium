
#include "y/mkl/algebra/tridiag.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"
#include "y/system/rtti.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/mkl/tao.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    static inline void TestTriDiag(Random::Rand &ran, const size_t nmax=10)
    {
        std::cerr << "-- using " << RTTI::Name<T>() << std::endl;
        typedef typename ScalarFor<T>::Type ScalarType;

        for(size_t n=1;n<=nmax;++n)
        {
            std::cerr << "--   n=" << n << std::endl;
            TriDiag<T>                 tr(n);
            CxxArray<T,Memory::Pooled> u(n);
            CxxArray<T,Memory::Pooled> r(n);
            CxxArray<T,Memory::Pooled> v(n);
            CxxArray<T,Memory::Pooled> w(n);

            do
            {
                for(size_t i=1;i<=n;++i)
                {
                    tr.a[i] = Bring<T>::Get(ran);
                    tr.b[i] = Bring<T>::Get(ran);
                    tr.c[i] = Bring<T>::Get(ran);
                    r[i]    = Bring<T>::Get(ran);
                }
            }
            while( !tr.solve(u,r) );

            Matrix<T> M(n,n);
            tr.sendTo(M);

            M.mul(v,u);
            tr.mul(w,u);
            const ScalarType residue1 = Tao::Mod2(v,r);
            const ScalarType residue2 = Tao::Mod2(v,r);

            //std::cerr << "M=" << M << std::endl;
           // std::cerr << "r=" << r << std::endl;
           // std::cerr << "u=" << u << std::endl;
           // std::cerr << "v=" << v << std::endl;
           // std::cerr << "w=" << w << std::endl;
            std::cerr << "--     residue1=" << residue1 << std::endl;
            std::cerr << "--     residue2=" << residue2 << std::endl;
        }
        std::cerr << std::endl;

    }
}

Y_UTEST(algebra_tridiag)
{
    Random::Rand ran;

    TestTriDiag<float>(ran);
    TestTriDiag<double>(ran);
    TestTriDiag<long double>(ran);


    TestTriDiag< XReal<float> >(ran);
    TestTriDiag< XReal<double> >(ran);
    TestTriDiag< XReal<long double> >(ran);

    TestTriDiag< Complex<float> >(ran);
    TestTriDiag< Complex<double> >(ran);
    TestTriDiag< Complex<long double> >(ran);

    TestTriDiag< Complex< XReal<float> > >(ran);
    TestTriDiag< Complex< XReal<double> > >(ran);
    TestTriDiag< Complex< XReal<long double> > >(ran);


    TestTriDiag<apq>(ran,5);


}
Y_UDONE()
