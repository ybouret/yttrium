#include "y/chemical/library.hpp"
#include "y/chemical/reactive/components.hpp"
#include "y/chemical/types.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(init)
{
    Library         lib;
    const Species & h = lib("H+",1);
    const Species & w = lib("HO-",-1);

    std::cerr << lib << std::endl;
    
    for(unsigned nu=1;nu<=3;++nu)
    {
        const Actor a(nu,h);
        std::cerr << a << std::endl;
    }

    {
        Actors reac;
        std::cerr << reac << std::endl;

        reac(1,h);
        reac(2,w);

        std::cerr << reac << std::endl;
    }

    {
        Components water("water",1);
        water(1,h);
        water(1,w);
        std::cerr << water.name << ":" << water.reac << water.Mark << water.prod <<  std::endl;
    }


    Y_SIZEOF(Entity);
    Y_SIZEOF(Species);
    Y_SIZEOF(Actor);
}
Y_UDONE()
