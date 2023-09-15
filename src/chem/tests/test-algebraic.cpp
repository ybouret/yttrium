#include "y/chem/algebraic.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/mkl/algebra/rank.hpp"

using namespace Yttrium;

Y_UTEST(algebraic)
{
    Random::Rand ran;
    const size_t N = 3;
    const size_t M = 5;
    Matrix<int> Nu(N,M);

    do
    {
        LightArray<int> arr = Nu.asArray();
        for(size_t i=arr.size();i>0;--i)
        {
            arr[i] = ran.in<int>(-2,2);
        }
    }
    while( N != MKL::Rank::Of(Nu) );

    std::cerr << "Nu=" << Nu << std::endl;
    Matrix<unsigned> Q;
    Chemical::Algebraic::Compute(Q,Nu);
    std::cerr << "Q=" << Q << std::endl;

    Chemical::Algebraic::Weight::List W;
    Chemical::Algebraic::Compute(W,Nu);


}
Y_UDONE()

