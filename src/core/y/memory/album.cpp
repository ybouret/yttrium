#include "y/memory/album.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/lockable.hpp"

namespace Yttrium
{


    namespace Memory
    {

        Album:: Album() :
        pages(NULL),
        pages_()
        {
            Lockable &giant = Lockable::Giant();
            pages = static_cast<Pages*>( OutOfReach::Zero(pages_,sizeof(pages_)) );
            pages -= Pages::MinShift;
            for(unsigned shift=Pages::MinShift;shift<=Pages::MaxShift;++shift)
            {
                new ( &pages[shift] ) Pages(shift,giant);
            }
        }

        Album:: ~Album() noexcept
        {
            for(unsigned shift=Pages::MaxShift;shift>=Pages::MinShift;--shift)
            {
                (void) OutOfReach::naught( &pages[shift] );
            }
        }

        Pages & Album:: operator[](const unsigned shift) noexcept
        {
            assert(shift>=Pages::MinShift);
            assert(shift<=Pages::MaxShift);
            return pages[shift];
        }


    }

}

