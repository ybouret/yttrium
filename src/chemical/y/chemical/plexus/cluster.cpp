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
        EList(),
        Party(),
        species(),
        Nu(),
        Qm(),
        laws(),
        conserved(),
        unbounded(),
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



    }

}


