

#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/plexus/injector.hpp"
#include "y/stream/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(plexus)
{
    Weasel::Compiler &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    (void) clusters.K(0);

    clusters.showK(std::cerr);

    {
        OutputFile fp("clusters.dot");
        GraphViz::Vizible::Enter(fp, "G");
        size_t cid=0;
        for(const Cluster *cl=clusters->head;cl;cl=cl->next,++cid)
        {
            cl->viz(fp,cid);
        }
        GraphViz::Vizible::Leave(fp);
    }

    GraphViz::Vizible::RenderPNG("clusters.dot", true);

    Y_SIZEOF(Cluster);
    Y_SIZEOF(Clusters);

    const size_t    m = lib->size();
    Vector<xreal_t> C0(m,0);
    Vector<xreal_t> dC(m,0);

    Species::Conc(C0,ran,0.3,0.5);

    lib(std::cerr,"[",C0,"]");

    Injector injector(clusters);
    injector(clusters,C0,dC,xml);

    lib(std::cerr << "C1=","\t[",C0,"]");
    lib(std::cerr << "dC=","\t[",dC,"]");


}
Y_UDONE()
