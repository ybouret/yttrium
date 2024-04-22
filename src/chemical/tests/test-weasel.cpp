
#include "y/chemical/weasel.hpp"
#include "y/chemical/reactive/clusters.hpp"
#include "y/utest/run.hpp"

#include "y/stream/libc/output.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

       
        


    }
}

Y_UTEST(weasel)
{
    Weasel &      weasel = Weasel::Instance();
    Library       lib;
    LuaEquilibria eqs;
    std::cerr << weasel.callSign() << std::endl;

    for(int i=1;i<argc;++i)
    {
        weasel( Jive::Module::OpenData("data",argv[i]), lib, eqs);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    {
        OutputFile fp("eqs.dot");
        GraphViz::Vizible::Enter(fp, "G");

        for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
        {
            (*it)->viz(fp);
        }

        for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
        {
            (*it)->viz(fp,SubLevel);
            (*it)->vizLink(fp,SubLevel);
        }

        GraphViz::Vizible::Leave(fp);
    }
    GraphViz::Vizible::Render("eqs.png", "eqs.dot");


    bool verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);
    (void) cls.K(0);
    std::cerr << eqs << std::endl;

    for(const Cluster *cl=cls.head;cl;cl=cl->next)
    {
        std::cerr << *cl << std::endl;
    }


    Y_SIZEOF(Chemical::Cluster);
    Y_SIZEOF(Chemical::Clusters);

}
Y_UDONE()

