



#include "y/chemical/reactive/clusters.hpp"

#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(metrics)
{
    Y_SIZEOF(Cluster);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);

    Y_SIZEOF(Species);
    Y_SIZEOF(Equilibrium);
    Y_SIZEOF(ConstEquilibrium);
    Y_SIZEOF(Entity);
    Y_SIZEOF(Actor);

}
Y_UDONE()