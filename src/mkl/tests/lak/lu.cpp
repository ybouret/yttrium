


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
        for(size_t n=1;n<=2;++n)
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



        }
    }
}

Y_UTEST(lak_lu)
{
    Random::Rand    ran;
    MKL::LU<double> lu0;
    MKL::LU<double> lu(10);

    testLU<double>(ran);



}
Y_UDONE()
