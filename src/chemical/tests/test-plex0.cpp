
#include "y/chemical/plexus.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/string/env.hpp"

#include "y/counting/combination.hpp"






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

Y_UTEST(plex0)
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




    //std::cerr << "lib=" << lib << std::endl;
    //std::cerr << "rep=" << rep << std::endl;

    plexus.cls.graphViz("cs");



    const size_t m = lib->size();
    XVector      C0(m,0); // concentration
    XVector      C1(m,0); // errors ?


    Display       display;
    Reactor::Proc callback = display;

    for(size_t k=m;k>0;--k)
    {
        Combination comb(m,k);
        do
        {
            std::cerr << std::endl;
            Library::Concentrations(C0,ran);
            for(size_t i=comb.size();i>0;--i) C0[ comb[i] ] = 0;
            std::cerr << C0 << std::endl;
            C1.ld(C0);
            plexus.rxn(xml,C1,&callback);
            std::cerr << C0 << " -> " << C1 << std::endl;

        } while(comb.next());

        std::cerr << std::endl;
        Library::Concentrations(C0,ran);
        std::cerr << C0 << std::endl;
        C1.ld(C0);
        plexus.rxn(xml,C1,&callback);
        std::cerr << C0 << " -> " << C1 << std::endl;
    }






}
Y_UDONE()

