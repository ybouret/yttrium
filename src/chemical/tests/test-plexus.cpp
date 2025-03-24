

#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/reactor.hpp"

#include "y/chemical/weasel.hpp"
#include "y/sequence/vector.hpp"

#include "y/utest/run.hpp"

#include "y/random/mt19937.hpp"


namespace Yttrium
{
    namespace Chemical
    {



      

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

    if(false)
    {
        Library::Concentrations(C0,ran);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        Aftermath am;
        for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
        {
            const Equilibrium &eq = **it;
            const xreal_t      eK = eq(cls.K,TopLevel);
            std::cerr << "solving " << eq.name << std::endl;
            lib.Concentrations(C0,ran);
            C.ld(C0);
            am(eq,eK,C,TopLevel,C0,TopLevel);
        }
    }

    //Reactor::EmitProfiles = true;
    Reactor::MonitorScore = true;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Library::Concentrations(C0,ran,0.5);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
        Reactor reactor(*cl);
        reactor(xml,C0,cls.K);
    }




    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Reactor);

}
Y_UDONE()

