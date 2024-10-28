
#include "y/lingo/lexical/rule.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Rule:: ~Rule() noexcept
            {
                assert(0!=motif);
                delete motif;
                Coerce(motif) = 0;
            }

            Rule:: Rule(const Caption & id,
                        Pattern * const p) noexcept :
            Entity(id,AsCaption),
            motif(p),
            next(0),
            prev(0)
            {

            }

        }

    }

}
