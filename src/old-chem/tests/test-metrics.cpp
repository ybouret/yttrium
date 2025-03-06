



#include "y/chemical/plexus/mixes.hpp"
#include "y/chemical/plexus/conservation/warden.hpp"
#include "y/chemical/reactive/equilibrium/hybrid.hpp"

#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(metrics)
{

    std::cerr << "-- Base Classes" << std::endl;
    Y_SIZEOF(Species);
    Y_SIZEOF(Equilibrium);
    Y_SIZEOF(Entity);
    Y_SIZEOF(ConstEquilibrium);
    Y_SIZEOF(HybridEquilibrium);
    Y_SIZEOF(Actor);
    std::cerr << std::endl;

    std::cerr << "-- Mix(es)" << std::endl;
    Y_SIZEOF(Mix);
    Y_SIZEOF(Mixes);
    Y_SIZEOF(Solver);
    std::cerr << std::endl;

    std::cerr << "-- Conservations" << std::endl;
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Conservation::Act);
    Y_SIZEOF(Conservation::Acts);
    Y_SIZEOF(Conservation::Authority);
    Y_SIZEOF(Conservation::Warden);
    std::cerr << std::endl;




}
Y_UDONE()
