#include "y/lingo/pattern/all.hpp"

namespace Yttrium
{
    namespace Lingo
    {


        static inline
        Pattern * Optim(Or * const p)
        {
            AutoPtr<Or> motif = p;
            motif->optimize();
            motif->noMultiple();
            return motif.yield();
        }

        Pattern * Pattern:: Optimize(Pattern * const p)
        {
            assert(0!=p);
            AutoPtr<Pattern> motif = p;

            switch(motif->uuid)
            {
                    // joker
                case Optional::UUID: motif->as<Optional>()->optimize(); break;
                case Repeated::UUID: motif->as<Repeated>()->optimize(); break;
                case Counting::UUID: motif->as<Counting>()->optimize(); break;

                    // logic
                case Or:: UUID: return Optim( motif.yield()->as<Or>() );
                default:
                    break;
            }

            return motif.yield();
        }
    }

}


