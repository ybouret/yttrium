
#include "y/kemp/sora.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Kemp;

Y_UTEST(kemp_sora)
{
    Random::ParkMiller ran;

    Vector<apq> Q;
    for(size_t n=0;n<=5;++n)
    {
        Q.free();
        for(size_t i=0;i<n;++i) Q << apq(10,10,ran);
        std::cerr << Q << std::endl;
        const Natural g = Sora::CommonDenominator(Q);
        std::cerr << "g=" << g << std::endl;
        std::cerr << Sora::Simplify(Q) << std::endl;
        
    }


}
Y_UDONE()
