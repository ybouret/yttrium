#include "y/chemical/library.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(eq)
{
    Library lib;

    const Species &h = lib("H^+");
    const Species &w = lib("HO^-");

    std::cerr << "h=" << h << std::endl;
    std::cerr << "w=" << w << std::endl;

    std::cerr << "lib=" << lib << std::endl;

    Actors actors;

    actors(h);
    actors(2,w);

    std::cerr << actors << std::endl;

    const Actors other = actors;

    std::cerr << other << std::endl;
}
Y_UDONE()
