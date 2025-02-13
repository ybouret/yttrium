

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
        Ortho::VCache         vcache = new Ortho::Vector::Cache(metrics);

        {
            Ortho::Family         F(metrics,vcache);
            Ortho::Family         sub(metrics,vcache);
            CxxArray<int> V(dims);

            do {
                for(size_t i=dims;i>0;--i) V[i] = ran.in<int>(-5,5);
                Y_ASSERT(F.includes(sub));
                if(F.wouldAccept(V))
                {
                    sub.ld(F);
                    F.expand();
                    std::cerr << "F=" << F << std::endl;
                    Y_ASSERT(F.includes(sub));
                    Y_ASSERT(!sub.includes(F));
                }
            } while(F.quality != Ortho::Generating);
            sub.reset();
            std::cerr << "#vcache=" << (*vcache)->size << std::endl;
        }
        std::cerr << "#vcache=" << (*vcache)->size << std::endl;
    }

}
Y_UDONE()
