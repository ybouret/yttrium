

#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_ortho)
{

    Random::ParkMiller ran;
    for(size_t dims=1;dims<=6;++dims)
    {
        std::cerr << std::endl << "dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::Cache          cache = new Ortho::Vector::Cache(metrics);
        Ortho::Family         F(metrics,cache);
        CxxArray<int> V(dims);

        do {


            for(size_t i=dims;i>0;--i) V[i] = ran.in<int>(-5,5);
            if(F.wouldAccept(V))
            {
                F.expand();
                std::cerr << "F=" << F << std::endl;
            }
        } while(F.quality != Ortho::Generating);

    }

}
Y_UDONE()
