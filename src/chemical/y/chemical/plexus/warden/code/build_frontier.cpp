#include "y/chemical/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Warden:: buildFrontier(const SingleFrontier &limiting,
                                    const Frontiers      &required)
        {
            assert(required.size>0);        // number of frontiers
            assert(limiting->size>0);       // number of limiting species
            assert(limiting.xi.mantissa>0); // positive limiting extent

            best.free();
            const  Frontier *prev = 0;
            for(const FNode *node=required.head;node;node=node->next)
            {
                const Frontier &F = **node;
                switch( Sign::Of(F.xi,limiting.xi) )
                {
                    case Negative:
                        prev = &F; // almost to this one
                        continue;

                    case __Zero__: // numerical match
                        Coerce(best.xi) = limiting.xi;
                        best << limiting;
                        best << F;
                        return;

                    case Positive:
                        break; // will stop
                }
                break; // stop
            }

            if(0!=prev)
            {
                //----------------------------------------------------------
                //
                // found a required before limiting, will solve up to it
                //
                //----------------------------------------------------------
                Coerce(best.xi) = prev->xi;
                best << *prev;
            }
            else
            {
                //----------------------------------------------------------
                //
                // no requirement, best partial effort
                //
                //----------------------------------------------------------
                Coerce(best.xi) = limiting.xi;
                best   << limiting;
            }


        }

    }

}

