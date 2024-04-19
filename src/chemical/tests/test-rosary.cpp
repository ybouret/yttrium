
#include "y/chemical/rosary.hpp"
#include "y/chemical/reactive/actors.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(rosary)
{
    Rosary & rosary = Rosary::Instance();
    Library       lib;
    LuaEquilibria eqs;
    std::cerr << rosary.callSign() << std::endl;

    for(int i=1;i<argc;++i)
    {
        rosary( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;
    
}
Y_UDONE()

