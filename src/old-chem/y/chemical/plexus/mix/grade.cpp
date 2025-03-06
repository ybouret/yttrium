
#include "y/chemical/plexus/mix.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Mix::Grade:: ~Grade() noexcept {}

        Mix:: Grade:: Grade(const EList &eqs, const Genus &genus) :
        prodOnly(),
        reacOnly(),
        oneSided(),
        limiting(),
        floating()
        {
            for(const ENode *en=eqs.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                switch(eq.attr)
                {
                    case Nebulous: continue;
                    case ProdOnly: Coerce(prodOnly.book) += eq; Coerce(oneSided.book) += eq; break;
                    case ReacOnly: Coerce(reacOnly.book) += eq; Coerce(oneSided.book) += eq; break;
                    case Definite:
                        if(genus.isLimiting(eq)) Coerce(limiting.book) += eq;
                        else                     Coerce(floating.book) += eq;
                        break;
                }
            }

            Coerce(prodOnly).compile();
            Coerce(reacOnly).compile();
            Coerce(oneSided).compile();
            Coerce(limiting).compile();
            Coerce(floating).compile();
        }

    }

}


