
#include "y/mkl/lak/ortho-space.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "../../../core/tests/main.hpp"

using namespace Yttrium;
using namespace MKL;


Y_UTEST(lak_ortho_space)
{
    Random::Rand ran;
    Matrix<apz> P(1,8);
    for(size_t i=1;i<=P.rows;++i)
    {
        for(size_t j=1;j<=P.cols;++j)
        {
            P[i][j] = ran.in<int>(-5,5);
        }
    }
    std::cerr << "P=" << P << std::endl;

    Matrix<apz> Q(8,8);
    OrthoSpace::Build(Q,P);
    std::cerr << "Q=" << Q << std::endl;
}
Y_UDONE()
