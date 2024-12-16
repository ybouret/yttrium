#include "y/chemical/plexus/mixes.hpp"
#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"

#include "y/chemical/plexus/solver.hpp"


#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

        

    }
}

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

    bool     verbose = true;
    XMLog    xml(verbose);
    Mixes    mixes(eqs,xml);
    
    std::cerr << "mixes=" << mixes << std::endl;

    const size_t M = lib->size();
    XVector      C0(M,0);

    Library::Conc(C0,ran,0.5);

    for(const Mix *mix=mixes->head;mix;mix=mix->next)
    {
        Solver solver(*mix);
        solver.run(xml, C0, TopLevel, eqs.K);
    }


#if 0
    Clusters clusters(eqs,xml);

    std::cerr << clusters << std::endl;

    
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    clusters.graphViz("plexus",true);

    return 0;
    
    const size_t M = lib->size();
    XVector C0(M,0);

    Aftermath am;
    for(const Cluster *cl=clusters->head;cl;cl=cl->next)
    {
        const Cluster &cluster = *cl;
        const size_t   n = cluster->size;
        const size_t   m = cluster->species.size;
        XVector C1(m,0);

        Y_XML_SECTION_OPT(xml, "Cluster","size=" << n);
        for(const ENode *en=cluster->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            const xReal        eK = eq.K(1.0);
            Y_XML_SECTION(xml,eq.name);
            eq.print(std::cerr) << ":" << Library::ToReal(eK) << std::endl;
            C0.ld(0);
            C1.ld(0);

            cluster(std::cerr << "C0=","\t[",C0,"]",Library::ToReal) << std::endl;
            {
                const Outcome outcome = am.solve(eq, eK, C1, SubLevel, C0, TopLevel);
                switch(outcome.st)
                {
                    case Blocked:
                        Y_XMLOG(xml, "Blocked");
                        break;

                    case Running:
                        cluster(std::cerr << "C1=","\t[",C1,"]",Library::ToReal) << std::endl;
                        Y_XMLOG(xml,"activity = " << Library::ToReal(eq.activity(am.xmul, eK, C1, SubLevel)) );
                        break;
                }
            }

            for(size_t iter=0;iter<1;++iter)
            {
                Library::Conc(C0,ran);
                cluster(std::cerr << "C0=","\t[",C0,"]",Library::ToReal) << std::endl;
                C1.ld(0);
                {
                    const Outcome outcome = am.solve(eq, eK, C1, SubLevel, C0, TopLevel);
                    switch(outcome.st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "Blocked");
                            break;

                        case Running:
                            cluster(std::cerr << "C1=","\t[",C1,"]",Library::ToReal) << std::endl;
                            Y_XMLOG(xml,"activity = " << Library::ToReal(eq.activity(am.xmul, eK, C1, SubLevel)) );
                            break;
                    }
                }

            }

        }
    }
#endif

}
Y_UDONE()
