
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
        next(0),
        prev(0)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Conservation &cons)
        {
            os << "0=d_(";
            os << cons.actors.toString();
            os << ")";
            return os;
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


    }

}

