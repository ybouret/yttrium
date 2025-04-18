#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/equalizer.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/string/env.hpp"

#include "y/data/list/cloneable.hpp"


#include "y/chemical/plexus/initial/axiom/electroneutrality.hpp"
#include "y/chemical/plexus/initial/axiom/fixed-concentration.hpp"
#include "y/chemical/plexus/initial/design.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {
            
           


        }

    }
}


using namespace Yttrium;
using namespace Chemical;

namespace
{
    struct Display
    {
        void operator()(const XReadable &C0,
                        const xreal_t    S0)
        {
            std::cerr << "score=" << std::setw(24) << S0.str() << " @" << C0 << std::endl;
        }
    };
}

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

    bool           verbose = Environment::Flag("VERBOSE");
    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);


    
    std::cerr << "lib=" << lib << std::endl;


    cls.graphViz("cs");

    

    const size_t m = lib->size();
    XVector      C0(m,0); // concentration

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    Equalizer eqz(cls);
    eqz(xml,C0);

    lib.show(std::cerr << "C1=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;




    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactor::EmitProfiles = false;
    Reactors cs(cls);

    Display            display;
    Reactor::Proc cb = display;
    cs(xml,C0,&cb);

    lib.show(std::cerr << "C2=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    Y_SIZEOF(Initial::Axiom);
    Y_SIZEOF(Initial::FixedConcentration);
    Y_SIZEOF(Initial::ElectroNeutrality);
    Y_SIZEOF(Initial::Design);

    Initial::Design design("init");

    design.add(new Initial::ElectroNeutrality());
    design.add(new Initial::FixedConcentration(lib["Na^+"],0.001) );

    design.build(xml,C0,lib,cls);

    //eqz(xml,C0);




}
Y_UDONE()

