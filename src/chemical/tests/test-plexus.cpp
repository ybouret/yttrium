#include "y/chemical/plexus.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/string/env.hpp"







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
    Repertory  rep;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,rep,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    lib.latch();
    bool           verbose = Environment::Flag("VERBOSE");
    XMLog          xml(verbose);
    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactor::EmitProfiles = false;

    Plexus plexus(xml,lib,eqs,0.0);



    
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "rep=" << rep << std::endl;

    plexus.cls.graphViz("cs");

    

    const size_t m = lib->size();
    XVector      C0(m,0); // concentration
    XVector      dC(m,0); // errors ?

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    plexus.eqz(xml,C0);
    lib.show(std::cerr << "C1=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    plexus.eqz.query(dC);
    lib.show(std::cerr << "dC=", "\t[", dC, "]", xreal_t::ToString ) << std::endl;


    return 0;
    
    Display            display;
    Reactor::Proc callback = display;

    for(Repertory::ConstIterator it=rep->begin();it!=rep->end();++it)
    {
        const Initial::Axioms &axioms = *it;
        std::cerr << "[[ " << axioms << " ]]" << std::endl;
        plexus(xml,C0,axioms,&callback);
    }

    //lib.show(std::cerr << "C2=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;








}
Y_UDONE()

