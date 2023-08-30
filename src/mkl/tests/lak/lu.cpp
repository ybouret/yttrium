


#include "y/mkl/lak/lu.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"
#include <typeinfo>

using namespace Yttrium;
using namespace MKL;


namespace
{
    template <typename T>
    static inline void testLU(Random::Bits &ran)
    {

        MKL::LU<T> lu;

        for(size_t n=1;n<=3;++n)
        {
            std::cerr << std::endl;
            std::cerr << "-- n = " << n << " --" << std::endl;
            Matrix<T> a(n,n);
            Matrix<T> a0(n,n);

        FILL:
            FillMatrix(a,ran);
            //std::cerr << "a=" << a << std::endl;
            a0.assign(a);
            if(!lu.build(a)) goto FILL;

            CxxArray<T,Memory::Dyadic> r(n); FillWritable(r,ran); // rhs
            CxxArray<T,Memory::Dyadic> b(r); lu.solve(a,b);       // b = a^-1 * r
            CxxArray<T,Memory::Dyadic> p(n); a0(p,b);             // p = a*b = r

            typename ScalarFor<T>::Type delta = 0, zero = 0;
            for(size_t i=n;i>0;--i)
            {
                delta += Fabs<T>::Of(p[i]-r[i]);
            }
            delta /= n;
            std::cerr << "delta=" << delta << std::endl;

            if( typeid(T) == typeid(apq) )
            {
                Y_CHECK(delta<=zero);
            }

#if 0
            Matrix<T> I(n,n);
            for(size_t i=1;i<=n;++i) I[i][i] = T(1);

            std::cerr << "I=" << I << std::endl;
            lu.solve(a,I);
            std::cerr << "I=" << I << std::endl;

            I.ld(0);
            lu.invert(a,I);
            std::cerr << "I=" << I << std::endl;
            
            std::cerr << "detA=" << lu.determinant(a) << std::endl;

            Matrix<T> adj(n,n);
            lu.adjoint(adj,a0);
            std::cerr << "a0=" << a0  << std::endl;
            std::cerr << "aa=" << adj << std::endl;
#endif

        }
    }
}

Y_UTEST(lak_lu)
{
    Random::Rand    ran;
    MKL::LU<double> lu0;
    MKL::LU<double> lu(10);

    testLU<double>(ran);
    testLU< XReal<float> >(ran);


    testLU<apq>(ran);

    testLU< Complex<long double> >(ran);

}
Y_UDONE()
