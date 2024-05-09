#include "y/chemical/reactive/cluster/constellation.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        
        ClusterConstellation:: ClusterConstellation(Equilibria        &eqs,
                                                    const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterCombinatorics(eqs,fragment,topK,xml),
        hasOnlyReac(),
        hasOnlyProd(),
        controllers()
        {

            static const char here[] = "Chemical::Cluster::Constellation";
            Y_XML_SECTION_OPT(xml, here, " eqs='" << size << "'");


            for(const ENode *node=head;node;node=node->next)
            {
                const Equilibrium &eq = **node;
                assert(eq->size()>0);
                assert(eq.reac.size+eq.prod.size==eq->size());

                if(xml.verbose)
                {
                    eqfmt.pad(xml() << " (*) " << eq, eq) << " : ";
                    eqfmt.print(*xml, eq) << " @ ";
                }

                if(eq.reac.size<=0)
                {
                    assert(eq.prod.size>0);
                    Coerce(hasOnlyProd) << eq;
                    if(xml.verbose) *xml << "hasOnlyProd" << std::endl;
                    continue;
                }

                if(eq.prod.size<=0)
                {
                    assert(eq.reac.size>0);
                    Coerce(hasOnlyReac) << eq;
                    if(xml.verbose) *xml << "hasOnlyReac" << std::endl;
                    continue;
                }

                assert(eq.reac.size>0);
                assert(eq.prod.size>0);

                AutoPtr<Controller> cntl = new Controller(eq,conserved);
                if(cntl->components.reac.size>0 && cntl->components.prod.size>0)
                {
                    if(xml.verbose) *xml << "regulates";
                    eqfmt.print(*xml,cntl->components) << std::endl;
                }
                else
                {
                    if(xml.verbose) *xml << "redundant" << std::endl;
                }
 

            }


        }
        
        ClusterConstellation:: ~ClusterConstellation() noexcept
        {
        }



    }
}
