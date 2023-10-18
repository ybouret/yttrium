
#include "y/chem/plexus/conservation.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Conservation:: ~Conservation() noexcept {}

        Conservation:: Conservation() noexcept :
        Object(),
        Proxy<const Actors>(),
        actors(),
        normSq(0),
        nrm2(0),
        next(0),
        prev(0)
        {
        }


        String Conservation:: toString() const
        {
            return "d_(" + actors.toString() + ")";
        }

        std::ostream & operator<<(std::ostream &os, const Conservation &cons)
        {
            return os << cons.toString();
        }




        Conservation::ConstInterface & Conservation:: surrogate() const noexcept
        {
            return actors;
        }

        void Conservation:: add(const unsigned nu, const Species &sp)
        {
            assert(nu!=0);
            for(Actor *a=actors.head;a;a=a->next)
            {
                const Species &mine = a->sp;
                if(&mine == &sp)
                    throw Specific::Exception("Conservation","multiple species '%s'",sp.name.c_str());
            }
            actors.pushTail( new Actor(sp,nu) );
            normSq      += nu*nu;
            Coerce(nrm2) = double(normSq);
        }

        bool Conservation:: linkedTo(const Conservation &other) const noexcept
        {
            //std::cerr << "check " << *this << " and " << other << std::endl;
            for(const Actor *ac=other->head;ac;ac=ac->next)
            {
                if( actors.has(ac->sp) ) return true;
            }
            return false;
        }


        xreal Conservation:: excess(const Readable<xreal> &C0, XAdd &xadd) const
        {
            xadd.free();
            for(const Actor *ac=actors.head;ac;ac=ac->next)
            {
                const Species &sp = ac->sp;
                xadd.insert( C0[sp.indx[TopLevel]] * ac->xn );
            }
            return xadd.sum();
        }


    }

}

