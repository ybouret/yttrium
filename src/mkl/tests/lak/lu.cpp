


#include "y/mkl/lak/lu.hpp"
#include "y/utest/run.hpp"
#include "../../../core/tests/main.hpp"
#include "y/container/cxx-array.hpp"

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

            Matrix<T> a(n,n);
            Matrix<T> a0(n,n);
        FILL:
            FillMatrix(a,ran);
            std::cerr << "a=" << a << std::endl;
            a0.assign(a);
            if(!lu.build(a)) goto FILL;

            CxxArray<T,Memory::Dyadic> r(n);
            FillWritable(r,ran);
            CxxArray<T,Memory::Dyadic> b(r);
            std::cerr << "r=" << r << std::endl;
            lu.solve(a,b);
            std::cerr << "b=" << b << std::endl;

            CxxArray<T,Memory::Dyadic> p(n);
            a0(p,b);
            std::cerr << "p=" << p << std::endl;

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
