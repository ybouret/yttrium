

#include "y/apex/api/ortho/family.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Apex;


Y_UTEST(apex_ortho)
{

    Random::ParkMiller ran;
    for(size_t dims=1;dims<=1;++dims)
    {
        std::cerr << std::endl << "dims=" << dims << std::endl;
        const Ortho::Metrics  metrics(dims);
        Ortho::QCache         cache = new Ortho::QVector::Cache(metrics);
        Ortho::QFamily        family(metrics,cache);
        CxxArray<int> V(dims);
        for(size_t i=dims;i>0;--i) V[i] = ran.in<int>(-5,5);
        if(family.wouldAccept(V))
        {

        }

    }

}
Y_UDONE()
