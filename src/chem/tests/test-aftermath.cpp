

#include "y/chemical/reactive/aftermath.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(aftermath)
{
    Random::ParkMiller ran;

    Library    lib;
    Equilibria eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[1]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    XVector C( lib->size(), 0 );


    Library::Conc(C,ran);

    lib(std::cerr << "C=","[",C,"]") << std::endl;
    lib(std::cerr << "C=","[",C,"]",Library::ToReal) << std::endl;



}
Y_UDONE()
