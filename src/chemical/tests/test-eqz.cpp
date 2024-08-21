#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/equalizer.hpp"
#include "y/chemical/plexus/janitor.hpp"

#include "y/sort/heap.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(eqz)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    XMLog           &xml = plexus.xml;
    //const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);
    XVector dC(C0);



    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Equalizer eqz(*cl);

        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");
        lib(std::cerr << "C0=","\t[",C0,"]");


        eqz.run(C0, TopLevel,xml);

#if 0
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            F(C0,TopLevel,eq,cl->conserved.book);
            cl->uuid.pad(std::cerr << eq.name, eq) << ":" << F << std::endl;
        }
#endif

        //lib(std::cerr << "C1=","\t[",C0,"]");



    }




    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
}
Y_UDONE()
