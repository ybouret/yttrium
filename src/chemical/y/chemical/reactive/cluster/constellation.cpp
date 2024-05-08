#include "y/chemical/reactive/cluster/constellation.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        static inline
        bool AreRoaming(const Actors &actors, const AddressBook &conserved)
        {
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if(conserved.search(a->sp)) return false;
            }
            return true; //! all actors are unbounded
        }

        ClusterConstellation:: ClusterConstellation(Equilibria        &eqs,
                                                    const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterCombinatorics(eqs,fragment,topK,xml),
        roamingBoth(),
        roamingReac(),
        roamingProd()
        {

            static const char here[] = "Chemical::Cluster::Constellation";
            Y_XML_SECTION_OPT(xml, here, " eqs='" << size << "'");

            static const unsigned ROAMING_NONE = 0x00;
            static const unsigned ROAMING_REAC = 0x01;
            static const unsigned ROAMING_PROD = 0x02;
            static const unsigned ROAMING_BOTH = ROAMING_REAC | ROAMING_PROD;

            for(const ENode *node=head;node;node=node->next)
            {
                const Equilibrium &eq = **node;
                assert(eq->size()>0);
                assert(eq.reac.size+eq.prod.size==eq->size());

                if(xml.verbose)
                {
                    eqfmt.pad(xml() << eq, eq) << " : ";
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

            }


        }
        
        ClusterConstellation:: ~ClusterConstellation() noexcept
        {
        }



    }
}
