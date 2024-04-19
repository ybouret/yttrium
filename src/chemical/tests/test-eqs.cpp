

#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/reactive/equilibrium/constant.hpp"

#include "y/chemical/species/library.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(eqs)
{
    Library        lib;
    const Species &proton  = lib("H+",1);
    const Species &hydroxy = lib("HO-",-1);
    const Species &ah      = lib("AH",0);
    const Species &am      = lib("A-",-1);

    std::cerr << lib << std::endl;

    Equilibria eqs;
    {
        Equilibrium &water = eqs( "water", 1e-14 );
        water(1,proton);
        water(1,hydroxy);
    }

    {
        Equilibrium &acid = eqs( "acid", 1e-4);
        acid(-1,ah);
        acid(1,am);
        acid(1,proton);
    }

    eqs.finalize();

    std::cerr << eqs << std::endl;

}
Y_UDONE()
