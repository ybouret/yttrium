
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
        const char * const EqScheme = "set18";

        Cluster::Cluster( XMLog                         &xml,
                         const ClusterContent::Pointer &ptr,
                         Equilibria                    &eqs,
                         XWritable                     &tlK) :
        ClusterCombinatorics(xml,ptr,eqs,tlK),
        next(0),
        prev(0),
        uuid(0),
        spColor( (*this)->species->size, AsCapacity ),
        eqColor( (*this)->equilibria->size, AsCapacity )
        {
            {
                Strings &colors = Coerce(spColor);
                for(const SNode *sn = (*this)->species->head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    const String   descr = Species::Color(SpScheme,sp.indx[SubLevel]);
                    const String   color = "color=" + descr + ",fontcolor=" + descr;
                    colors << color;
                    //std::cerr << color << std::endl;
                }
            }

            {
                Strings &colors = Coerce(eqColor);
                for(const ENode *en = (*this)->equilibria->head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const String   descr = Species::Color(EqScheme,eq.indx[SubLevel]);
                    const String   color = "color=" + descr + ",fontcolor=" + descr;
                    colors << color;
                    //std::cerr << color << std::endl;
                }
            }

            
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
                    (**en).viz(fp,color);
                }

                fp << "}\n";
            }
        }
    }

}


