
#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Cluster::CallSign = "Cluster";

        
        Cluster:: ~Cluster() noexcept
        {
            
        }

        const char * const SpScheme = "dark28";
        const char * const EqScheme = "set28";
        const char * const CnScheme = "paired9";


        static inline void
        addToColors(const Cluster::Strings &    colors,
                    const char * const          scheme,
                    const size_t                cindex)
        {
            const String   descr = GraphViz::Vizible::Color(scheme,cindex);
            const String   color = "color=" + descr + ",fontcolor=" + descr;
            Coerce(colors) << color;
        }

        Cluster::Cluster( XMLog                         &xml,
                         const ClusterContent::Pointer &ptr,
                         Equilibria                    &eqs,
                         XWritable                     &tlK) :
        ClusterCombinatorics(xml,ptr,eqs,tlK),
        canons(xml,*ordinance,*definite,unbounded),
        next(0),
        prev(0),
        uuid(0),
        spColor( (*this)->species->size, AsCapacity ),
        eqColor( (*this)->equilibria->size, AsCapacity ),
        cnColor( ordinance->size, AsCapacity)
        {

            for(const SNode *sn = (*this)->species->head;sn;sn=sn->next)
                addToColors(spColor,SpScheme, (**sn).indx[SubLevel]);

            for(const ENode *en = (*this)->equilibria->head;en;en=en->next)
                addToColors(eqColor,EqScheme,(**en).indx[SubLevel]);

            for(const Conservation::Law *ln=ordinance->head;ln;ln=ln->next)
                addToColors(cnColor,CnScheme,ln->subId);
            
        }
        
        std::ostream & operator<<(std::ostream &os, const Cluster &cl)
        {
            os << '{' << " cluster_" << cl.uuid << "/" << cl->species << ' ';
            const EList &elist = *cl->equilibria;
            if(elist.size>>0)
            {
                os << std::endl;
                for(const ENode *en=elist.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    cl->display(os << '\t',eq, Coerce(eq).K(0)) << std::endl;
                }
            }
            os << '}';
            return os;
        }


        void Cluster:: viz(OutputStream &fp,
                           const size_t   numOrder) const
        {
            if(numOrder<=order.size())
            {
                fp("subgraph cluster_%u",uuid) << "{\n";
                
                for(const SNode *sn = (*this)->species->head;sn;sn=sn->next)
                {
                    const Species &      sp    = **sn;
                    const String * const color = & sp(spColor,SubLevel);
                    sp.viz(fp,color,conserved.has(sp));
                }
                
                for(const ENode *en=order[numOrder].head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const String * const color = & eq(eqColor,SubLevel);
                    eq.viz(fp,color,characterize(eq));
                }

                if(numOrder==1)
                {
                    for(const Conservation::Law *law=ordinance->head;law;law=law->next)
                    {
                        law->viz(fp,&cnColor[law->subId]);
                    }
                }

                fp << "}\n";
            }
        }
    }

}


