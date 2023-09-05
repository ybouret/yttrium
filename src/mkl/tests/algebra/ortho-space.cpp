
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "../../../core/tests/main.hpp"
#include "y/mkl/algebra/rank.hpp"

using namespace Yttrium;
using namespace MKL;


Y_UTEST(lak_ortho_space)
{
    Random::Rand ran;
    if(false)
    {
        Matrix<apz> P(1,8);

        for(size_t i=1;i<=P.rows;++i)
        {
            for(size_t j=1;j<=P.cols;++j)
            {
                P[i][j] = ran.in<int>(-5,5);
            }
        }
        std::cerr << "P=" << P << std::endl;

        Matrix<apz> Q;
        OrthoSpace::Build(Q,P);
        std::cerr << "Q=" << Q << std::endl;
        Y_CHECK(7==MKL::Rank::Of(Q));
    }


    const size_t dim = 8;
    for(size_t nr=1;nr<=dim;++nr)
    {
        std::cerr << "rank(P)=" << nr << std::endl;
        Matrix<apz> P(nr,dim);
        do
        {
            for(size_t i=1;i<=P.rows;++i)
            {
                for(size_t j=1;j<=P.cols;++j)
                {
                    P[i][j] = ran.in<int>(-5,5);
                }
            }
        }
        while(MKL::Rank::Of(P)!=nr);
        std::cerr << "P=" << P << std::endl;
        Matrix<apz> Q;
        OrthoSpace::Build(Q,P);
        std::cerr << "Q=" << Q << std::endl;
        Y_ASSERT(dim-nr==MKL::Rank::Of(Q));
    }

}
Y_UDONE()
