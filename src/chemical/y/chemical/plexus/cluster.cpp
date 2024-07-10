#include "y/chemical/plexus/cluster.hpp"
//#include "y/system/exception.hpp"
#include "y/graphviz/color-scheme.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: ~Cluster() noexcept
        {
        }

        Cluster:: Cluster(const Equilibrium &first) :
        EList(),
        Party(),
        species(),
        Nu(),
        Qm(),
        laws(),
        conserved(),
        unbounded(),
        order(),
        next(0),
        prev(0)
        {
            *this << first;
        }



        bool Cluster:: accept(const Equilibrium &eq) const noexcept
        {
            for(const ENode *en=head;en;en=en->next)
            {
                if( eq.linkedTo( **en) ) return true;
            }
            return false;
        }

        bool Cluster:: accept(const Cluster &cl) const noexcept
        {
            for(const ENode *en=head;en;en=en->next)
            {
                if(cl.accept(**en)) return true;
            }
            return false;
        }

        const Cluster & Cluster:: operator*() const noexcept { return *this; }

        void Cluster:: show(std::ostream &os) const
        {
            os << '#' << size << ' ' << '{' << std::endl;
            for(const ENode *en=head;en;en=en->next)
            {
                display(os,**en) << std::endl;
            }
            os << '}' << std::endl;
        }


        void Cluster:: show(std::ostream &os, const XReadable &K) const
        {
            os << '#' << size << ' ' << '{' << std::endl;
            for(const ENode *en=head;en;en=en->next)
            {
                display(os,**en,K) << std::endl;
            }
            os << '}' << std::endl;
        }

        void Cluster:: viz(OutputStream &fp, const size_t cid) const
        {
            const String spColor = "black";

            fp << "subgraph cluster_" << Formatted::Get("%u",unsigned(cid)) << "{\n";

            // write species
            for(const SNode *sn=conserved.list.head;sn;sn=sn->next)
            {
                (**sn).viz(fp,spColor,true);
            }

            for(const SNode *sn=unbounded.list.head;sn;sn=sn->next)
            {
                (**sn).viz(fp,spColor,false);
            }

            // write eqs
            const EList &el = (*order)[1];
            for(const ENode *node=el.head;node;node=node->next)
            {
                const Equilibrium &eq    = **node;
                const String       color = GraphViz::Vizible::Color("set18",eq.indx[TopLevel]);
                eq.viz(fp,color);
            }

            if(laws.isValid())
            {
                // write cons
                size_t il = 1;
                for(const CLaw *law = laws->head; law; law=law->next, ++il)
                {
                    const String color = GraphViz::Vizible::Color("accent8",il);;
                    law->viz(fp, color);
                }
            }

            fp << "}\n";

        }


    }

}


