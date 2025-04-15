
#include "y/chemical/plexus/initial/player.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            const char * const Player:: First = "FirstPlayer";
            const char * const Player:: Extra = "ExtraPlayer";


            Player:: Player(const int      n,
                            const Species &s) noexcept :
            cf(n),
            sp(s),
            next(0),
            prev(0)
            {

            }

            Player:: ~Player() noexcept
            {
            }

        }

    }

}
