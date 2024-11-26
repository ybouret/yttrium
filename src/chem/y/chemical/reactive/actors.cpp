
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Company:: ~Company() noexcept {}

        Company:: Company() noexcept :
        Entity(),
        Actor::List(),
        Assembly()
        {
            
        }


        bool Company:: lookFor(const Species &sp) const noexcept
        {
            for(const Actor *mine=head;mine;mine=mine->next)
            {
                if( &sp == & (mine->sp) ) return true;
            }
            return false;
        }


        void Company:: recruit(Actor * const actor)
        {
            assert(0!=actor);
            assert(!lookFor(actor->sp));
            AutoPtr<Actor> guard(actor);
            {
                String newName = name;
                if(size>0) newName += " + ";
                newName += actor->name;
                Coerce(name).swapWith(newName);
            }
            enroll( *pushTail( guard.yield() ) );
        }

        const String & Company:: key() const noexcept
        {
            return name;
        }



    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: Actors() noexcept
        {
        }

        Actors:: ~Actors() noexcept
        {
        }


        Actors::ConstInterface & Actors::surrogate() const noexcept { return company; }

        void Actors:: operator()(const Species &sp)
        {
            assert(!company.lookFor(sp));
            company.recruit( new Actor(sp) );
        }

        void Actors:: operator()(const unsigned nu, const Species &sp)
        {
            assert(!company.lookFor(sp));
            assert(nu>0);
            company.recruit( new Actor(nu,sp) );
        }

        std::ostream &   operator<<(std::ostream &os, const Actors &actors)
        {
            return os << (actors->name);
        }
    }

}
