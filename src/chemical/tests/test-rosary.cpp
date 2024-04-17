
#include "y/chemical/lang/rosary.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(rosary)
{
    Rosary & rosary = Rosary::Instance();
    Library  lib;
    std::cerr << rosary.callSign() << std::endl;

    const Species &proton = lib("H+",1);

    for(int i=1;i<argc;++i)
    {
        rosary( Jive::Module::OpenData("data",argv[i]), lib);
    }

}
Y_UDONE()

