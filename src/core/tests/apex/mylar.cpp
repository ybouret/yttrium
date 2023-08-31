#include "y/apex/mylar.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/bits.hpp"


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

    Q.free();
    Q << 0 << -1;
    std::cerr << Q << " -> ";
    Apex::Mylar::Univocal(Q);
    std::cerr << Q << std::endl;

    Q.free();
    for(size_t i=6;i>0;--i)
    {
        Q << apq(1+ran.leq(10),1+ran.leq(10),ran);
    }
    std::cerr << Q << std::endl;
    Apex::Mylar::Univocal(Q);
    std::cerr << Q << std::endl;


}
Y_UDONE()
