#include "y/chemical/plexus/mixes.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/chemical/plexus/solver.hpp"


#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

        

    }
}

Y_UTEST(plexus)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Mixes    mixes(xml,eqs);

    std::cerr << "mixes=" << mixes << std::endl;

    
    const size_t M = lib->size();
    XVector      C0(M,0);

    Library::Conc(C0,ran,0.5);

    mixes.solve(xml, C0, TopLevel, eqs.K);



}
Y_UDONE()
