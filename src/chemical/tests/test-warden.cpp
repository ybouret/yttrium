#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/plexus/warden.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;



Y_UTEST(warden)
{
    Chemical::Weasel &        weasel = Chemical::Weasel::Instance();
    Chemical::Library         lib;
    Chemical::LuaEquilibria   eqs;

    for(int i=1;i<argc;++i)
    {
        weasel( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }
    
    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);
    cls.graphViz("system");
    Chemical::Conservation::Warden warden(cls);


}
Y_UDONE()
