
#include "y/chemical/plexus/reactors.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"


#include "y/chemical/plexus/conservation/warden.hpp"

#include "y/chemical/plexus/equalizer/extents.hpp"
#include "y/system/exception.hpp"




namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

         
         




        }

    }

}



using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{

    Random::MT19937 ran;
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library    lib;
    Equilibria eqs;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool           verbose = true;
    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);


    
    std::cerr << "lib=" << lib << std::endl;
    cls.graphViz("cs");


    const size_t m = lib->size();
    XVector      C0(m,0); // concentration
    Summator     I0(m);

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    AddressBook vanishing;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        vanishing.free();
        for(const Conservation::Canon *canon=cl->canons.head;canon;canon=canon->next)
        {
            Conservation::Warden warden(*cl,*canon);
            warden.fix(xml, C0, I0, TopLevel, vanishing);
        }
        vanishing.display<Species>(std::cerr) << std::endl;
    }

    lib.show(std::cerr << "C1=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    return 0;

    XVector      C(m,0);

    for(size_t iter=0;iter<100;++iter)
    {
        Library::Concentrations(C0,ran,0.1,0.5);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        //wardens(xml,C0);

        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        for(const Cluster *cl=cls->head;cl;cl=cl->next)
        {
            //Equalizer            eqz(*cl);
            //eqz(xml,C0);
        }
    }


    return 0;


    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    //lib.show(std::cerr << "I0=", "\t[", I0, "]", xreal_t::ToString ) << std::endl;

    return 0;

    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactors cs(cls);
    cs(xml,C0);




    
    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);

    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Conservation::Canon);
    Y_SIZEOF(Conservation::Canons);

    Y_SIZEOF(Reactor);


}
Y_UDONE()

