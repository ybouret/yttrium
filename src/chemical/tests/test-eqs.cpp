
#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/equilibria.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(eqs)
{
    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library lib;
    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        XCode        code = weasel.parse( Lingo::Module::Open(data,data) );
        Y_CHECK(code->defines<Weasel>());
        GraphViz::Vizible::DotToPng("code.dot", *code);
        for( XNode *xnode=code->branch().head;xnode;xnode=xnode->next)
        {
            if( xnode->defines<Equilibrium>() )
            {
                //ConstEquilibrium eq(lib,xnode);
                //std::cerr << eq << std::endl;
                AutoPtr<Equilibrium> eq = weasel.compile(lib,xnode,1);
                std::cerr << eq << " =" << eq->K(0) << std::endl;
            }
        }

    }


}
Y_UDONE()
