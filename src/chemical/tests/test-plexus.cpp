
#include "y/chemical/plexus/reactors.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"


#include "y/chemical/plexus/equalizer.hpp"




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

    Equalizer::Proceedings eqz(cls);

#if 0
    Equalizer::Banks banks;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Equalizer::Proceeding eqz(*cl,banks);
        eqz.session(xml,C0,I0);
    }
#endif

    eqz.session(xml, C0, I0);

    lib.show(std::cerr << "C1=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    Y_SIZEOF(Conservation::Warden);
    Y_SIZEOF(Equalizer::TwoSided);
    Y_SIZEOF(Equalizer::OneSided);
    Y_SIZEOF(Equalizer::KList);
    Y_SIZEOF(Equalizer::GList);
    Y_SIZEOF(Equalizer::Proceeding);

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

