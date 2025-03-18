
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/plexus/cluster/combinatorics.hpp"
#include "y/chemical/plexus/cluster/builder.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"
#include "y/chemical/weasel.hpp"

#include "y/sequence/vector.hpp"

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
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << *cl << std::endl;
    }

    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);
    Y_SIZEOF(ClusterBuilder);
    Y_SIZEOF(ClusterKnot);
    Y_SIZEOF(Conservation::Rule);
    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);

}
Y_UDONE()

