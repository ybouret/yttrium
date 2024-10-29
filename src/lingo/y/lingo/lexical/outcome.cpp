#include "y/lingo/lexical/outcome.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Lingo
    {

        namespace Lexical
        {
            Outcome:: Outcome(const Unit::Feat _feat, const Unit::Spot _spot) noexcept :
            type(Unit::Regular),
            spot(_spot),
            args()
            {
                Coerce(args.regular.feat) = _feat;
            }

            Outcome:: Outcome(const Unit::Spot _spot) noexcept :
            type(Unit::Control),
            spot(_spot),
            args()
            {

            }

            Outcome:: ~Outcome() noexcept
            {
            }

            Outcome:: Outcome(const Outcome &_) noexcept :
            type(_.type),
            spot(_.spot),
            args()
            {
                memcpy((void*)&args,&_.args,sizeof(args));
            }
        }

    }

}

