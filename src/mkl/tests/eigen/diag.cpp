
#include "y/mkl/eigen/sort.hpp"
#include "y/mkl/eigen/diagonalization.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/tao/seq/level3.hpp"
#include "y/mkl/api.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline void testDiag( Random::Bits &ran )
    {

        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
        MKL::Eigen::Diagonalization<T> diag;

        for(size_t n=1;n<=3;++n)
        {
            Matrix<T> a(n,n);
            Vector<T> wr(n,0), wi(n,0);
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    a[i][j] = ran.symm<float>();
                }
            }
            std::cerr << "a=" << a << std::endl;
            const MKL::Eigen::Values<T> *values = diag.eig(a);
            if(values)
            {
                std::cerr << "wr=" << values->wr << std::endl;
                std::cerr << "wc=" << values->wc << std::endl;
            }
            else std::cerr << "Couldn't diag!!" << std::endl;
        }
        std::cerr << std::endl;
    }

}

Y_UTEST(eigen_diag)
{
    Random::Rand ran;

    testDiag< float >( ran );
    testDiag< double >( ran );


}
Y_UDONE()

