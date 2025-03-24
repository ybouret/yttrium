

#include "y/chemical/plexus/reactors.hpp"

#include "y/chemical/weasel.hpp"
#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

#include "y/random/mt19937.hpp"



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

    bool verbose = true;

    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << *cl << std::endl;
    }

    const size_t m = lib->size();
    XVector      C0(m,0);
    XVector      C(m,0);


    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    
    Reactors cs(cls);
    Library::Concentrations(C0,ran,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    
    cs(xml,C0);





    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Reactor);

}
Y_UDONE()

