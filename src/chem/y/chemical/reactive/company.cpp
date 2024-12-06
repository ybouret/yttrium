#include "y/chemical/reactive/company.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Company:: ~Company() noexcept {}

        Company:: Company(const NameForm info) noexcept :
        Entity(), Actor::List(), name(), form(info)
        {

        }

        Company:: Company(const Company &_) :
        Entity(), Actor::List(_),name(_.name),form(_.form)
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
                switch(form)
                {
                    case NameAsNormalSpecies: newName +=       actor->name;       break;
                    case NameAsConcentration: newName += '[' + actor->name + ']'; break;
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


        String Company:: html() const
        {
            String res;
            const Actor *a = head;
            if(0!=a)
            {
                res = a->html(form);
                for(a=a->next;a;a=a->next)
                {
                    res += '+' + a->html(form);
                }
            }
            return res;
        }

    }

}
