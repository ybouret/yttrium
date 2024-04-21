
#include "y/chemical/rosary.hpp"
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

Y_UTEST(rosary)
{
    Rosary & rosary = Rosary::Instance();
    Library       lib;
    LuaEquilibria eqs;
    std::cerr << rosary.callSign() << std::endl;

    for(int i=1;i<argc;++i)
    {
        rosary( Jive::Module::OpenData("data",argv[i]), lib, eqs);
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
            (*it)->viz(fp);
            (*it)->vizLink(fp);
        }

        GraphViz::Vizible::Leave(fp);
    }
    GraphViz::Vizible::Render("eqs.png", "eqs.dot");


    bool verbose = true;
    XMLog xml(verbose);
    Chemical::Constants K;
    Chemical::Clusters  cls(eqs,K,xml);
    //K->adjust(eqs->size(),1);
    //std::cerr << cls << std::endl;
    (void) cls.K(0);
    std::cerr << eqs << std::endl;

    Y_SIZEOF(Chemical::Cluster);
    Y_SIZEOF(Chemical::Clusters);

}
Y_UDONE()

