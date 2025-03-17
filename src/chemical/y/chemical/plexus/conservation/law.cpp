

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Law:: ~Law() noexcept
            {
            }

            static unsigned ActorsNorm2(const Actors &actors)
            {
                apn n2 = 0;
                for(const Actor *a=actors->head;a;a=a->next)
                {
                    n2 += Squared(a->nu);
                }
                return n2.cast<unsigned>(actors.name->c_str());
            }

            Law:: Law(Rule * const rule) :
            Actors(rule->in),
            norm2(ActorsNorm2(*rule)),
            next(0),
            prev(0)
            {
                Actors::Exchange(*this,*rule);
                latch();
            }
        }
    }

}

