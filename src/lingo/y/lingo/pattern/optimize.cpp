#include "y/lingo/pattern/all.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        Pattern * Pattern:: Optimize(Pattern * const p)
        {
            assert(0!=p);
            AutoPtr<Pattern> motif = p;

            switch(motif->uuid)
            {
                case Optional::UUID: motif->as<Optional>()->optimize(); break;
                case Repeated::UUID: motif->as<Repeated>()->optimize(); break;
                case Counting::UUID: motif->as<Counting>()->optimize(); break;

                default:
                    break;
            }

            return motif.yield();
        }
    }

}


