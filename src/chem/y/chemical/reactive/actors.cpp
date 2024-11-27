
#include "y/chemical/reactive/actors.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Company:: ~Company() noexcept {}

        Company:: Company() noexcept :
        Entity(), Actor::List(), Assembly(), name()
        {
            
        }

        Company:: Company(const Company &_) :
        Entity(), Actor::List(_), Assembly(_), name(_.name)
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


        const Actor & Company:: recruit(Actor * const actor)
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
            return *actor;
        }

        const String & Company:: key() const noexcept
        {
            return name;
        }

        void Company:: xch(Company &_) noexcept
        {
            swapWith(_);
            CoerceSwap(maxKeySize,_.maxKeySize);
            Coerce(name).swapWith(Coerce(_.name));
        }


    }

}

namespace Yttrium
{
    namespace Chemical
    {
        Actors:: Actors() noexcept : Proxy<const Company>(), company()
        {
        }

        Actors:: Actors(const Actors &_)  : Proxy<const Company>(), company(_.company)
        {
        }

        Actors:: ~Actors() noexcept
        {
        }


        Actors::ConstInterface & Actors::surrogate() const noexcept { return company; }

        const Actor & Actors:: operator()(const Species &sp)
        {
            assert(!company.lookFor(sp));
            return company.recruit( new Actor(sp) );
        }

        const Actor & Actors:: operator()(const unsigned nu, const Species &sp)
        {
            assert(!company.lookFor(sp));
            assert(nu>0);
            return company.recruit( new Actor(nu,sp) );
        }

        std::ostream &   operator<<(std::ostream &os, const Actors &actors)
        {
            return os << (actors->name);
        }


    }

}
