
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: ~Actors() noexcept {}

        Actors:: Actors() noexcept :
        Entity(""),
        Proxy<const Actor::List>(),
        actors() {}

        Actors:: Actors(const Actors &ac) :
        Entity(ac.name),
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


        void Actors:: add(Actor *a)
        {
            assert(0!=a);

            // check no multiple species
            AutoPtr<Actor> guard(a);
            for(const Actor *mine=actors.head;mine;mine=mine->next)
            {
                if( & (mine->sp) == &(a->sp) )
                    throw Specific::Exception("Chemical::Actors", "multiple species '%s'", a->sp.name.c_str());
            }

            // change name
            if(actors.size<=0)
                growName(a->sp.name);
            else
            {
                const String ext = '+' + a->sp.name;
                growName(ext);
            }

            // store new actor
            actors.pushTail( guard.yield() );

        }
       
        void Actors:: add(const unsigned nu, const Species &sp)
        {
            add( new Actor(nu,sp) );
        }



    }

}


