#include "y/mkl/eigen/sort.hpp"
#include "y/mkl/eigen/jacobi.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/sequence/vector.hpp"
#include "../../../core/tests/main.hpp"
#include "y/mkl/tao/seq/level3.hpp"
#include "y/mkl/api.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline void doJacobi(Random::Bits &ran)
    {
        const String &ts = RTTI::Name<T>();
        std::cerr << "Jacobi<" << ts << ">" << std::endl;
        MKL::Eigen::Jacobi<T> J;
        MKL::Tao::ComputeMod2<T> mod2;
        MKL:: Antelope::Caddy<T> xm;

        for(size_t n=1;n<=4;++n)
        {
            Matrix<T> a(n,n);
            Matrix<T> v(n,n);
            Vector<T> d(n,0);

            FillMatrix(a,ran);
            for(size_t i=1;i<=n;++i)
                for(size_t j=1;j<=i;++j)
                    a[i][j] = a[j][i];

            std::cerr << "a =" << a << std::endl;
            if(!J.build(a,d,v))
            {
                std::cerr << "Failure!" << std::endl;
                continue;
            }
            //std::cerr << "a1=" << a << std::endl;
            MKL::Eigen::Sort(d,v, Comparison::Decreasing<T>);

            std::cerr << "d=diagm(" << d << ")" << std::endl;
            std::cerr << "v=" << v << std::endl;
            std::cerr << "v*d*v'-a" << std::endl;
            const Matrix<T> vT(TransposeOf,v);
            Matrix<T>       dvT(n,n);
            MKL::Tao::DiagMatMul(dvT,d,vT);
            Matrix<T>       P(n,n);
            MKL::Tao::MatMul(P,v, dvT, xm);
            //std::cerr << "P=" << P << std::endl;

            {
                const LightArray<T> arr = P.asArray();
                const LightArray<T> brr = a.asArray();
                const T             sq2 = mod2(arr,brr)/T(n*n);
                const T             rms = MKL::Sqrt<T>::Of(sq2);
                std::cerr << "rms=" << rms << std::endl;
            }

            std::cerr << std::endl;
        }

    }
}

Y_UTEST(eigen_jacobi)
{
    Random::Rand ran;
    doJacobi<float>(ran);
    doJacobi<double>(ran);
    doJacobi<long double>(ran);
    doJacobi< XReal<float> >(ran);
    doJacobi< XReal<double> >(ran);
    doJacobi< XReal<long double> >(ran);
}
Y_UDONE()

