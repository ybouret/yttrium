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
        ClusterCombinatorics(eqs,fragment,topK,xml)
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
                    Coerce(hasOnlyProd) += eq;
                    if(xml.verbose) *xml << "hasOnlyProd" << std::endl;
                    continue;
                }

                if(eq.prod.size<=0)
                {
                    assert(eq.reac.size>0);
                    Coerce(hasOnlyReac) += eq;
                    if(xml.verbose) *xml << "hasOnlyReac" << std::endl;
                    continue;
                }

                assert(eq.reac.size>0);
                assert(eq.prod.size>0);

                Regulator regulator(eq,conserved);
                if(regulator->reac.size>0 && regulator->prod.size>0)
                {
                    if(xml.verbose) *xml << "regulates";
                    eqfmt.print(*xml,*regulator) << std::endl;
                }
                else
                {
                    if(xml.verbose) *xml << "redundant" << std::endl;
                }
#if 0
                Components regulator;
                for(Components::ConstIterator it=eq->begin();it!=eq->end();++it)
                {
                    const Component &cc = *it;
                    const Species   &sp = cc.sp;
                    if(conserved.search(sp)) regulator(cc);
                }

                if(regulator.reac.size>0 && regulator.prod.size>0)
                {
                    if(xml.verbose) *xml << "regulates" << std::endl;
                }
                else
                {
                    if(xml.verbose) *xml << "redundant" << std::endl;
                }
#endif

#if 0

                unsigned flag = ROAMING_NONE;
                if(AreRoaming(eq.reac,conserved)) flag |= ROAMING_REAC;
                if(AreRoaming(eq.prod,conserved)) flag |= ROAMING_PROD;
                switch(flag)
                {
                    case ROAMING_NONE: 
                        if(xml.verbose) *xml << "roamingNone" << std::endl;
                        Coerce(roamingNone) += eq;
                        break;

                    case ROAMING_REAC:
                        if(xml.verbose) *xml << "roamingReac" << std::endl;
                        Coerce(roamingReac) += eq;
                        break;

                    case ROAMING_PROD:
                        if(xml.verbose) *xml << "roamingProd" << std::endl;
                        Coerce(roamingProd) += eq;
                        break;

                    case ROAMING_BOTH:
                        if(xml.verbose) *xml << "roamingBoth" << std::endl;
                        Coerce(roamingBoth) += eq;
                        break;

                    default:
                        throw Specific::Exception(here, "corrupted <%s>", eq.name.c_str());

                }
#endif

            }


        }
        
        ClusterConstellation:: ~ClusterConstellation() noexcept
        {
        }



    }
}
