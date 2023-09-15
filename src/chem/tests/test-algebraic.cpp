#include "y/chem/algebraic.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;

Y_UTEST(algebraic)
{
    Random::Rand ran;
    size_t N = 3; if(argc>1) N = ASCII::Convert::To<size_t>(argv[1],"N");
    size_t M = 5; if(argc>2) M = ASCII::Convert::To<size_t>(argv[2],"M");


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

