
#include "y/chemical/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        const char * const Cluster::CallSign = "Cluster";

        
        Cluster:: ~Cluster() noexcept
        {
            
        }

        Cluster::Cluster( XMLog                         &xml,
                         const ClusterContent::Pointer &ptr,
                         Equilibria                    &eqs,
                         XWritable                     &tlK) :
        ClusterCombinatorics(xml,ptr,eqs,tlK),
        next(0),
        prev(0),
        uuid(0)
        {

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
    }

}


