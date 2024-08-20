#include "y/chemical/plexus/cluster.hpp"
#include "y/graphviz/color-scheme.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Roaming:: ~Roaming() noexcept {}

        Roaming:: Roaming() noexcept : reacOnly(), prodOnly(), standard() {}

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
        limited(),
        roaming(),
        order(0),
        topology(),
        attached(),
        detached(),
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
            for(const ENode *en=head;en;en=en->next) {
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

        size_t Cluster:: maxOrder() const noexcept
        {
            for(size_t i=order.size();i>1;--i)
            {
                if(order[i].size>0) return i;
            }
            return 1;
        }



        void Cluster:: vizSpecies(OutputStream &fp, const EList &el, const String &spColor) const
        {
            AddressBook book;
            for(const ENode *en=el.head;en;en=en->next)
            {
                (**en).record(book);
            }

            for(AddressBook::Iterator it=book.begin();it!=book.end();++it)
            {
                const Species &sp = *static_cast<const Species *>( *it );
                if(conserved.book.has(sp)) sp.viz(fp,spColor,true);
                if(unbounded.book.has(sp)) sp.viz(fp,spColor,false);
            }

            
        }

        void Cluster:: vizEqsList(OutputStream &fp, const EList &el) const
        {
            const String spColor = "black";
            vizSpecies(fp,el,spColor);
            for(const ENode *node=el.head;node;node=node->next)
            {
                const Equilibrium &eq    = **node;
                const String       color = GraphViz::Vizible::Color("set18",eq.indx[TopLevel]);
                eq.viz(fp,color);
            }
        }


        void Cluster:: vizProlog(OutputStream &fp, const size_t cid) const
        {
            fp << "subgraph cluster_" << Formatted::Get("%u",unsigned(cid)) << "{\n";
        }

        void Cluster:: vizEpilog(OutputStream &fp) const
        {
            fp << "}\n";
        }



        void Cluster:: viz(const size_t  ord, 
                           OutputStream &fp,
                           const size_t  cid) const
        {

            // check populated
            if(ord>order.size())
                return;
            const EList &el = order[ord];
            if(el.size <= 0) return;

            // write
            vizProlog(fp,cid);
            vizEqsList(fp,el);

            // specific
            if( 1==ord && laws.isValid() )
            {
                size_t il = 1;
                for(const CLaw *law = laws->head; law; law=law->next, ++il)
                {
                    const String color = GraphViz::Vizible::Color("accent8",il);;
                    law->viz(fp, color);
                }
            }
            vizEpilog(fp);
        }



    }

}


