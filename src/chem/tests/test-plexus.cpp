#include "y/chemical/reactive/clusters.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/chemical/reactive/conservation/law.hpp"


#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;



Y_UTEST(plexus)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);

    std::cerr << clusters << std::endl;

    
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;
    
    {
        OutputFile fp("plexus.dot");
        GraphViz::Vizible::Enter(fp,"G");
        for(const Cluster *cluster=clusters->head;cluster;cluster=cluster->next)
        {
            const Cluster &cl = *cluster;
            fp << "subgraph cluster_" << Formatted::Get("%u",cl.indx) << "{\n";

            // write species
            for(const SNode *sn=cl->species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                const String   color = sp.makeColor();
                sp.viz(fp, color.c_str(), 0);
            }

            // write components
            for(const ENode *en=cl->head;en;en=en->next)
            {
                const Components &cm    = **en;
                const String      color = cm.makeColor();
                cm.viz(fp, color.c_str(), 0);

            }

            // write laws
            for(const Conservation::Law *ln=cl.laws->head;ln;ln=ln->next)
            {
                const Conservation::Law &law = *ln;
                law.viz(fp);
            }

            fp << "}\n";
        }
        GraphViz::Vizible::Leave(fp);
    }

    GraphViz::Vizible::RenderPNG("plexus.dot",false);



}
Y_UDONE()
