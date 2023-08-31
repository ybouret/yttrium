



#include "y/apex/mylar.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/sequence/vector.hpp"

#include <cstdio>

using namespace Yttrium;


Y_UTEST(apex_mylar)
{
    Random::Rand ran;

    Vector<apq> Q;
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Q << apq(1,2);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;
    Q << apq(1,3);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Q << apq(1,7);
    std::cerr << "ComDen(" << Q << ")=" << Apex::Mylar::CommonDenominator(Q) << std::endl;

    Apex::Mylar::Simplify(Q);
    std::cerr << "Q=" << Q << std::endl;


}
Y_UDONE()
