#include "y/lingo/pattern.hpp"

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

                default:
                    break;
            }

            return motif.yield();
        }
    }

}


