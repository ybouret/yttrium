
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

    const Species &proton  = lib("H+",1);  
    const Species &hydroxy = lib("HO-",-1);
    lib.pad(std::cerr << "proton",proton)   << " = " << proton << std::endl;
    lib.pad(std::cerr << "hydroxy",hydroxy) << " = " << hydroxy << std::endl;

    return 0;

    for(int i=1;i<argc;++i)
    {
        rosary( Jive::Module::OpenData("data",argv[i]), lib);
    }

}
Y_UDONE()

