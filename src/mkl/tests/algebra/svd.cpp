
#include "y/mkl/eigen/sort.hpp"
#include "y/mkl/algebra/svd.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/sequence/vector.hpp"
#include "../../../core/tests/main.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline void doSVD(Random::Bits &ran)
    {
        const String &ts = RTTI::Name<T>();
        std::cerr << "SVD<" << ts << ">" << std::endl;

        MKL::SVD<T> svd;

        for(size_t m=1;m<=2;++m)
        {
            for(size_t n=1;n<=2;++n)
            {
                std::cerr << std::endl;

                std::cerr << "using LinearAlgebra" << std::endl;
                Matrix<T> a(m,n);
                Matrix<T> v(n,n);
                Vector<T> w(n,0);

                FillMatrix(a,ran);
                Matrix<T> u = a;

                std::cerr << "a=" << a << std::endl;
                if(!svd.build(u,w,v))
                {
                    std::cerr << "#failure" << std::endl;
                    continue;
                }

                std::cerr << "u=" << u << std::endl;
                std::cerr << "w=diagm(" << w << ")" << std::endl;
                std::cerr << "v=" << v << std::endl;


            }

        }


    }
}

Y_UTEST(algebra_svd)
{
    Random::Rand ran;


    doSVD<float>(ran);
    //doSVD<double>(ran);
    //doSVD<long double>(ran);


}
Y_UDONE()

