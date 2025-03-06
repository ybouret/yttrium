


#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(library)
{
    Library lib;

    const Species &h = lib("H^+");
    const Species &w = lib("HO^-");

    std::cerr << "h=" << h << std::endl;
    std::cerr << "w=" << w << std::endl;

    std::cerr << "lib=" << lib << std::endl;

    Random::ParkMiller ran;

    for(size_t i=0;i<10;++i)
    {
        std::cerr << Library::RanP(ran) << std::endl;
    }

    SList slist;
    SBank sbank;
    SRepo srepo(sbank);
}
Y_UDONE()
