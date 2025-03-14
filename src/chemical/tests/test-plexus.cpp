
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"

#include "y/chemical/weasel.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(plexus)
{

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

    bool verbose = true;

    XMLog          xml(verbose);
    ClusterBuilder cls(xml,eqs);

#if 1
    for(const ClusterNode *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << "cluster: " << *cl << std::endl;
        const ClusterContent::Pointer cc( &Coerce(**cl) );
        const ClusterConservations    ct(xml,cc);
    }
#endif

    std::cerr << "lib=" << lib << std::endl;

    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterType);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(ClusterBuilder);
    Y_SIZEOF(ClusterNode);

}
Y_UDONE()

