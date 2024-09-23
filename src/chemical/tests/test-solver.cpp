#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"
#include "y/chemical/plexus/solver.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(solver)
{

    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    XRealOutput::Mode = XRealOutput::Compact;

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

            //solver.run(C0, TopLevel, K, xml);

#if 1
            OutputFile fp("ff.dat");
            for(unsigned turn=1;turn<=1;++turn)
            {
                const Outcome ans = solver.process(C0, TopLevel, K, xml);
                if(1==turn)
                {
                    fp("0 %.15g\n", real_t(solver.ff0));
                }
                fp("%u",turn);
                fp(" %.15g",  real_t(solver.objFunc(C0,TopLevel)) );
                fp << '\n';
                lib(std::cerr << "C" << turn << "=","\t[",C0,"]");
                if(Solved==ans) break;
            }
#endif

        }
    }

    Y_SIZEOF(Vertex);
    Y_SIZEOF(Solver);


}
Y_UDONE()
