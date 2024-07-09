#include "y/chemical/plexus/cluster.hpp"
//#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Cluster:: ~Cluster() noexcept
        {
        }

        Cluster:: Cluster(const Equilibrium &first) :
        Proxy<const EList>(),
        Party(),
        eqs(),
        species(),
        Nu(),
        next(0),
        prev(0)
        {
            eqs << first;
        }

        Cluster:: ConstInterface & Cluster:: surrogate() const noexcept { return eqs; }


        bool Cluster:: accept(const Equilibrium &eq) const noexcept
        {
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                if( eq.linkedTo( **en) ) return true;
            }
            return false;
        }

        bool Cluster:: accept(const Cluster &cl) const noexcept
        {
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                if(cl.accept(**en)) return true;
            }
            return false;
        }

        


    }

}


