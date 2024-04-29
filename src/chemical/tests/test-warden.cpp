#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/plexus/warden.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;



Y_UTEST(warden)
{
    Random::Rand              ran;
    Chemical::Weasel &        weasel = Chemical::Weasel::Instance();
    Chemical::Library         lib;
    Chemical::LuaEquilibria   eqs;

    for(int i=1;i<argc;++i)
    {
        weasel( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }
    
    Vector<Chemical::xreal_t> C0(lib->size(),0),Injected(C0);
    for(size_t i=C0.size();i>0;--i)
    {
        C0[i] = Chemical::Species::Concentration(ran);
        if( ran.choice() ) C0[i] = - C0[i];
    }



    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);

    lib(std::cerr << "C=",C0) << std::endl;



    cls.graphViz("system");
    
    Chemical::Conservation::Warden warden(cls);

    warden(C0, Injected, cls.groups,xml);




}
Y_UDONE()
