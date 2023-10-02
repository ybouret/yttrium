#include "y/jive/lexical/action.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            Action:: ~Action() noexcept
            {

            }

            Message Action:: perform()
            {
                const Token &token = *motif;
                assert(token.size>0);
                return doing(token);
            }

            void Action:: cleanup() noexcept
            {
                motif->release();
            }


            Unit * Action:: produce()
            {
                Token &token = *motif;
                assert(token.size>0);
                assert(0!=token.head);
                Unit *unit = new Unit(*this,*(motif->head));
                unit->swapWith(token);
                return unit;
            }

        }
    }

}
