
#include "y/chemical/plexus/mixes.hpp"
#include "y/chemical/plexus/conservation/warden.hpp"

#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"



#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(warden)
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
    GraphViz::Vizible::DotToPngEx("warden.dot",mixes,1);

    const size_t M = lib->size();
    XVector      C0(M,0);

    Library::Conc(C0,ran,0.5,0.5);

    lib(std::cerr << "C0=","[",C0,"]", xReal::ToString) << std::endl;


}
Y_UDONE()
