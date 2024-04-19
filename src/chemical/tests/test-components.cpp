
#include "y/chemical/reactive/components.hpp"
#include "y/chemical/species/library.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(components)
{
    Library        lib;
    const Species &proton  = lib("H+",1);
    const Species &hydroxy = lib("HO-",-1);
    const Species &water   = lib("H2O",0);

    std::cerr << lib << std::endl;
    Components cmp;

    cmp(1,proton);
    cmp(1,hydroxy);
    cmp(-1,water);

    std::cerr << cmp << std::endl;

}
Y_UDONE()
