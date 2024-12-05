#include "y/chemical/reactive/company.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Company:: ~Company() noexcept {}

        Company:: Company(const bool bareName) noexcept :
        Entity(), Actor::List(), name(), bare(bareName)
        {

        }

        Company:: Company(const Company &_) :
        Entity(), Actor::List(_),name(_.name),bare(_.bare)
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
                if(bare)
                {
                    newName += actor->name;
                }
                else
                {
                    newName += '[' + actor->name + ']';
                }

                Coerce(name).swapWith(newName);
            }
            return * pushTail( guard.yield() );
        }

        const String & Company:: key() const noexcept
        {
            return name;
        }

        void Company:: xch(Company &_) noexcept
        {
            swapWith(_);
            Coerce(name).swapWith(Coerce(_.name));
        }


    }

}
