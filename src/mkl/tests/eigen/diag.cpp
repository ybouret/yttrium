
#include "y/mkl/eigen/diagonalization.hpp"
#include "y/mkl/algebra/svd.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/system/rtti.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/tao/seq/level3.hpp"
#include "y/mkl/api.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;

namespace
{
    template <typename T>
    static inline void testDiag( Random::Bits &ran, const size_t nmax)
    {

        std::cerr << "<" << RTTI::Name<T>() << ">" << std::endl;
        MKL::Eigen::Diagonalization<T> diag;
        //MKL::SVD<T>                    svd;

        for(size_t n=1;n<=nmax;++n)
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
            if(!values)
            {
                std::cerr << "Couldn't diag!!" << std::endl;
                continue;
            }

            std::cerr << "wr=" << values->wr << std::endl;
            std::cerr << "wc=" << values->wc << std::endl;


            const size_t nr = values->wr.size();
            if(nr>0)
            {
                std::cerr << "Looking for real eigenvector(s)" << std::endl;
                Vector<size_t> eIdx(nr,0);
                for(size_t i=1;i<=nr;++i) eIdx[i] = i;

                Vector<T> eVal(nr,0);
                Matrix<T> eVec(nr,n);

                diag.eigv(eVal,eVec,eIdx,a);
                for(size_t i=1;i<=nr;++i)
                {
                    std::cerr << "lam" << i << " = " << eVal[i] << "; v" << i << "=" << eVec[i] << std::endl;
                }

            }





        }
        std::cerr << std::endl;
    }

}

#include "y/text/ascii/convert.hpp"

Y_UTEST(eigen_diag)
{
    Random::Rand ran;

    size_t nmax = 4;
    if(argc>1) nmax = ASCII::Convert::To<size_t>(argv[1],"nmax");
    testDiag< float >( ran, nmax );
    return 0;

    testDiag< double >( ran, nmax);

    testDiag< long double >( ran, nmax);

    testDiag< XReal<float> >( ran, nmax );
    testDiag< XReal<double> >( ran, nmax );
    testDiag< XReal<long double> >( ran, nmax );



}
Y_UDONE()

