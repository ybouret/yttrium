#include "y/chemical/reactive/cluster.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[1]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


}
Y_UDONE()
