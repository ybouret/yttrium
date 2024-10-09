#include "y/chemical/plexus/device.hpp"
#include "y/chemical/plexus/wardens.hpp"

#include "y/chemical/plexus.hpp"


#include "y/utest/run.hpp"
#include "y/jive/module.hpp"


namespace Yttrium
{
    namespace Chemical
    {

       

       

    }
}

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(device)
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

        Device dev(*cl);

        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.1);
            lib(std::cerr << "C0=","\t[",C0,"]");
            ward(C0,dC,TopLevel,xml);
            lib(std::cerr << "C0=","\t[",C0,"]");

            dev(C0, TopLevel, K, xml);


        }

    }
}
Y_UDONE()
