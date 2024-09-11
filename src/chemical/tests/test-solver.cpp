#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"
#include "y/chemical/plexus/solver.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;
using namespace Chemical;


Y_UTEST(solver)
{

    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    Clusters        &cls = plexus.assemble();
    const XReadable &K   = cls.K(0);
    Wardens          ward(cls);

    const Library    &lib = plexus.lib;
    //const Equilibria &eqs = plexus.eqs;
    XMLog            &xml = plexus.xml;

    XVector C0(lib->size(),0);
    XVector dC(C0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Solver solver(*cl);

        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.1);
            lib(std::cerr << "C0=","\t[",C0,"]");
            ward(C0,dC,TopLevel,xml);
            lib(std::cerr << "C0=","\t[",C0,"]");

            for(size_t turn=1;turn<=5;++turn)
            {
                solver.process(C0, TopLevel, K, xml);
                lib(std::cerr << "C" << turn << "=","\t[",C0,"]");
            }

        }
    }




}
Y_UDONE()
