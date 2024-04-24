#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/clusters.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(warden)
{
    Weasel &      weasel = Weasel::Instance();
    Library       lib;
    LuaEquilibria eqs;

    for(int i=1;i<argc;++i)
    {
        weasel( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }
    
    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);
    
}
Y_UDONE()
