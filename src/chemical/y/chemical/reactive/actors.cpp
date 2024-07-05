
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: ~Actors() noexcept {}

        Actors:: Actors(const size_t level) noexcept :
        Entity("",level),
        Proxy<const Actor::List>(),
        actors() {}

        Actors:: Actors(const Actors &ac) :
        Entity(CopyOf,ac),
        Proxy<const Actor::List>(),
        actors(ac.actors)
        {
        }
        


        Actors::ConstInterface &Actors:: surrogate() const noexcept { return actors; }

        std::ostream & operator<<(std::ostream &os, const Actors &ac)
        {
            return os<<ac.name;
        }
       
        void Actors:: growName(const String &s)
        {
            String _ = name + s;
            Coerce(name).swapWith(_);
        }


        void Actors:: add(const unsigned nu, const Species &sp)
        {
            //__________________________________________________________________
            //
            // reserve actor
            //__________________________________________________________________
            Actor         *a = new Actor(nu,sp,actors.size+1);
            AutoPtr<Actor> guard(a);
           
            //__________________________________________________________________
            //
            // check no multiple species
            //__________________________________________________________________
            for(const Actor *mine=actors.head;mine;mine=mine->next)
            {
                if( & (mine->sp) == &(a->sp) )
                    throw Specific::Exception("Chemical::Actors", "multiple species '%s'", a->sp.name.c_str());
            }

            //__________________________________________________________________
            //
            // change name
            //__________________________________________________________________
            if(actors.size<=0)
                growName(a->name);
            else
            {
                const String ext = '+' + a->name;
                growName(ext);
            }

            //__________________________________________________________________
            //
            // store new actor
            //__________________________________________________________________
            actors.pushTail( guard.yield() );

        }
       



    }

}


