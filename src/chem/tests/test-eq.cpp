
#include "y/chemical/weasel.hpp"
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

    {
        Actors actors(NameAsNormalSpecies);

        actors(h);
        actors(2,w);

        std::cerr << actors << std::endl;

        const Actors other = actors;

        std::cerr << other << std::endl;
    }

    Components water("water",1);

    water.print(std::cerr) << std::endl;
    water.add(Product,h);
    water.print(std::cerr) << std::endl;
    water.add(Product,w);
    water.print(std::cerr) << std::endl;


}
Y_UDONE()
