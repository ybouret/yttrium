#include "y/chemical/reactive/cluster/topology.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterTopology:: ClusterTopology(const Fragment    &fragment,
                                          const Constants   &topK,
                                          XMLog             &xml) :
        Fragment(fragment),
        sharedK(topK),
        Nu(size,species.size),
        spset(),
        eqset(),
        eqfmt(),
        spfmt()
        {
            static const char here[] = "Chemical::Cluster::Topology";
            Y_XML_SECTION_OPT(xml, here, " eqs='" << size << "' species='" << species.size << "'");

            //------------------------------------------------------------------
            //
            // collect info from equilibria
            //
            //------------------------------------------------------------------
            for(const ENode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                Coerce(eqfmt).upgradeWith(eq);                   // format
                Coerce(eqset).record(eq);                        // table
                eq.fill(Coerce(Nu)[eq.indx[SubLevel]],SubLevel); // topology
            }
            Y_XMLOG(xml,"Nu   = " << Nu);

            if( size != MKL::Rank::Of(Nu) )
                throw Specific::Exception(here, "invalid system rank!!");

            //------------------------------------------------------------------
            //
            // collect info from species
            //
            //------------------------------------------------------------------
            for(const SNode *sn=species.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                Coerce(spfmt).updateWith(sp);
                if( ! Coerce(spset).record(sp) ) throw Specific::Exception(here,"unexpected species multiple sub-index!");
            }

        }

        ClusterTopology:: ~ClusterTopology() noexcept
        {
        }
    }

}

