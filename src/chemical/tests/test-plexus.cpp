
#include "y/chemical/plexus/clusters.hpp"
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

    Clusters cls(eqs);
    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        std::cerr << "cluster: " << *cl << std::endl;
    }
    std::cerr << "lib=" << lib << std::endl;
}
Y_UDONE()

