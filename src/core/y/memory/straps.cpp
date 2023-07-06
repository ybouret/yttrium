#include "y/memory/straps.hpp"

namespace Yttrium
{
    namespace Memory
    {

        Straps:: ~Straps() noexcept
        {

        }

        Straps:: Straps(Album &userAlbum) noexcept :
        ListOf<Strap>(),
        cache(0),
        empty(0),
        album(userAlbum)
        {
            
        }


    }
}
