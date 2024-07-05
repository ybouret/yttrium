#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/types.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(init)
{
    const Species h("H+",1);

    std::cerr << h << std::endl;

    for(unsigned nu=1;nu<=3;++nu)
    {
        const Actor a(nu,h);
        std::cerr << a << std::endl;
    }

    Actors reac;
    std::cerr << reac << std::endl;


}
Y_UDONE()
