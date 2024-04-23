#include "y/chemical/reactive/cluster.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(Equilibria        &eqs,
                          const Batch       &batch,
                          const Constants   &topK,
                          XMLog             &xml) :
        EList(batch),
        sharedK(topK),
        species(batch.species),
        Nu(size,species.size),
        Qm(),
        spset(),
        eqset(),
        eqfmt(),
        spfmt(),
        blend(),
        next(0),
        prev(0)
        {
            static const char here[] = "Chemical::Cluster";
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

        Cluster::  ~Cluster() noexcept {}


#if 0
        bool Cluster:: sharesSpeciesWith(const Equilibrium &rhs) const noexcept
        {
            for(const ENode *mine=head;mine;mine=mine->next)
            {
                const Equilibrium &lhs = **mine;
                if( lhs.sharesSpeciesWith(rhs) ) return true;
            }
            return false;
        }

        bool Cluster:: sharesSpeciesWith(const Cluster &other) const noexcept
        {
            for(const ENode *mine=head;mine;mine=mine->next)
            {
                if(other.sharesSpeciesWith(**mine)) return true;
            }
            return false;
        }

#endif

        void Cluster:: getK(const Real t)
        {
            Writable<XReal> &K = *sharedK;
            for(const ENode *node=head;node;node=node->next)
            {
                Equilibrium &eq = Coerce(**node);
                K[eq.indx[TopLevel]] = eq.K(t);
            }
        }


        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            os << "<Cluster species='" << cl.species.size << "' eqs='" << cl.size << "'>" << std::endl;
            for(const ENode *node=cl.head;node;node=node->next)
            {
                cl.eqfmt.display(os << "  ", **node) << std::endl;
            }
            os << "  (*) " << cl.species << std::endl;
            return os << "<Cluster/>";
        }


        void Cluster:: viz(OutputStream &fp, const size_t order, const size_t clusterIndex) const
        {
            assert(order>0);
            assert(order<=blend.size());
            const EList &eqs = blend[order];
            fp("subgraph cluster_%lu {\n", static_cast<unsigned long>(clusterIndex));
            for(const SNode *sn = species.head;sn;sn=sn->next)
            {
                (**sn).viz(fp);
            }

            for(const ENode *en = eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                eq.viz(fp,SubLevel);
                eq.vizLink(fp,SubLevel);
            }

            if(1==order)
            {
                for(const Conservation::Law *law=claws->head;law;law=law->next)
                {
                    law->viz(fp);
                }
            }

            fp << "}\n";
        }


    }

}
