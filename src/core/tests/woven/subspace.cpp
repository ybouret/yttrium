
#include "y/woven/subspace.hpp"
#include "y/utest/run.hpp"
#include "../main.hpp"


using namespace Yttrium;

Y_UTEST(woven_subspace)
{
    Random::Rand  ran;
    WOVEn::IProxy px;
    Matrix<int>   mu(4,3);

    for(size_t i=1;i<=mu.rows;++i)
    {
        for(size_t j=1;j<=mu.cols;++j)
        {
            mu[i][j] = ran.in<int>(-5,5);
        }
    }

    std::cerr << "mu=" << mu << std::endl;

    WOVEn::Subspace sub(mu,1,px);

    
}
Y_UDONE()

