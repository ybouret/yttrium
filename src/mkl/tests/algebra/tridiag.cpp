
#include "y/mkl/algebra/tridiag.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"
#include "y/system/rtti.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace Yttrium;
using namespace MKL;

namespace
{
    template <typename T>
    static inline void TestTriDiag(Random::Rand &ran, const size_t nmax=4)
    {

        for(size_t n=1;n<=nmax;++n)
        {
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
            for(size_t i=1;i<=n;++i)
                for(size_t j=1;j<=n;++j)
                    M[i][j] = tr(i,j);

            std::cerr << "M=" << M << std::endl;
            std::cerr << "r=" << r << std::endl;
            std::cerr << "u=" << u << std::endl;
            M.mul(v,u);
            std::cerr << "v=" << v << std::endl;
            tr.mul(w,u);
            std::cerr << "w=" << w << std::endl;


        }

    }
}

Y_UTEST(algebra_tridiag)
{
    Random::Rand ran;

    TestTriDiag<float>(ran);
    TestTriDiag<double>(ran);
    TestTriDiag<long double>(ran);

    //TestTriDiag<apq>(ran);


}
Y_UDONE()
