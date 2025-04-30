
#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/initial/design.hpp"

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

Y_UTEST(titration)
{

    Library    lib;
    Equilibria eqs;
    Repertory  rep;

    Chemical::Weasel  & weasel = Weasel::Instance();

    if(argc<=1) {
        std::cerr << "usage: " << program << " file.ini" << std::endl;
        return 1;
    }

    weasel(lib,eqs,rep,Lingo::Module::OpenFile(argv[1]));
    lib.latch();

    bool               verbose = Environment::Flag("VERBOSE");
    XMLog              xml(verbose);
    Plexus             plexus(xml,lib,eqs,0.0);
    Display            display;
    Reactor::Proc cb = display;

    Vector<xreal_t> Ca(lib->size(),0);
    plexus(xml,Ca,rep["Sa"],NULL);
    lib.show(std::cerr << "Ca=", "\t[", Ca, "]", xreal_t::ToString ) << std::endl;


}
Y_UDONE()
