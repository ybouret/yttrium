#include "y/chemical/reactive/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Cluster:: Cluster(const Equilibrium &eq,
                          const Constants   &topK) :
        EList(),
        sharedK(topK),
        species(),
        Nu(),
        spset(),
        eqset(),
        eqfmt(),
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
            for(const ENode *node=cl.head;node;node=node->next)
            {
                cl.eqfmt.display(os << "  ", **node) << std::endl;
            }
            return os;
        }

    }

}