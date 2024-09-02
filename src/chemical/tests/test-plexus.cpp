#include "y/chemical/plexus/warden.hpp"
#include "y/chemical/plexus.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(plexus)
{

    Plexus plexus(true);
    const Library    &lib = plexus.lib;
    const Equilibria &eqs = plexus.eqs;
    XMLog            &xml = plexus.xml;

#if 0
    plexus("@water @oxalic.*");

    std::cerr << "lib=" <<  lib << std::endl;
    std::cerr << "eqs=" <<  eqs << std::endl;
    Clusters &cls = plexus.assemble();


    XVector C0(lib->size(),0);
    const Species &ah2 = lib["OxH2"];
    const Species &ahm = lib["OxH-"];
    const Species &amm = lib["Ox--"];

    C0[ ah2.indx[TopLevel] ] =  1.0;
    C0[ ahm.indx[TopLevel] ] = -0.7;
    C0[ amm.indx[TopLevel] ] = -0.7;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {

        Warden warden(*cl);
        lib(std::cerr << "C0=","\t[",C0,"]");
        warden.run(C0,TopLevel,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");
    }

#else
    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    Clusters        &cls = plexus.assemble();
    //const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);
    XVector dC(C0);


    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {

        Warden warden(*cl);
        for(size_t iter=0;iter<1;++iter)
        {
            std::cerr << std::endl << "--" << std::endl;
            plexus.conc(C0,0.3,0.5);
            XVector     C1(C0);
            warden.prolog();
            lib(std::cerr << "C0=","\t[",C0,"]");
            warden.sanitize(C1,TopLevel,xml);
            warden.epilog(dC,TopLevel);
            lib(std::cerr << "C0=","\t[",C0,"]");
            lib(std::cerr << "C1=","\t[",C1,"]");
            lib(std::cerr << "dC=","\t[",dC,"]");

        }


    }
#endif


    Y_SIZEOF(Warden);
}
Y_UDONE()

