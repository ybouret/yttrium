
#include "y/mkl/algebra/svd.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/sequence/vector.hpp"
#include "../../../core/tests/main.hpp"
#include "y/mkl/tao/seq/level3.hpp"

using namespace Yttrium;


namespace
{
    template <typename T>
    static inline void doSVD(Random::Bits &ran)
    {
        const String &ts = RTTI::Name<T>();
        std::cerr << "SVD<" << ts << ">" << std::endl;

        MKL::SVD<T>              svd;
        MKL:: Antelope::Caddy<T> xm;

        for(size_t m=1;m<=4;++m)
        {
            for(size_t n=1;n<=4;++n)
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
                    std::cerr << "#failure n=" << n << ", m=" << m  << std::endl << std::endl;
                    continue;
                }


                MKL::SVD<T>::Sort(u, w, v);
                
                std::cerr << "u=" << u << std::endl;
                std::cerr << "w=diagm(" << w << ")" << std::endl;
                std::cerr << "v=" << v << std::endl;

                std::cerr << "a-u*w*v'" << std::endl;

                const Matrix<T> vT(TransposeOf,v);
                Matrix<T>       wvT(n,n);
                MKL::Tao::DiagMatMul(wvT,w,vT);
                Matrix<T> P(m,n);
                MKL::Tao::MatMul(P,u,wvT,xm);
                std::cerr << "P=" << P << std::endl;
            }

        }


    }
}

Y_UTEST(algebra_svd)
{
    Random::Rand ran;


    doSVD<float>(ran);
    doSVD<double>(ran);
    doSVD<long double>(ran);


    doSVD< XReal<float> >(ran);
    doSVD< XReal<double> >(ran);
    doSVD< XReal<long double> >(ran);

    return 0;

}
Y_UDONE()

