
#include "y/lingo/lexical/rule.hpp"
#include "y/system/exception.hpp"

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
                        Pattern * const p,
                        const Action   &a) noexcept :
            Entity(id,AsCaption),
            motif(p),
            action(a),
            next(0),
            prev(0)
            {

            }

            void Rule:: ErrorFeeblePattern(const String &rid)
            {
                throw Specific::Exception("Lexical::Rule", "feeble pattern for '%s'", rid.c_str());
            }

        }

    }

}
