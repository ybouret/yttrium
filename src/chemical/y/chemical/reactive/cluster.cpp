#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(const Equilibrium &eq) :
        EList(),
        species(),
        nu(),
        spfmt(),
        next(0),
        prev(0)
        {
            (*this) << eq;
        }

        Cluster::  ~Cluster() noexcept {}


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

      

    }

}
