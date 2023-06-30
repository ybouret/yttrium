#include "y/memory/book.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/lockable.hpp"

namespace Yttrium
{


    namespace Memory
    {

        Book:: Book() :
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

        Book:: ~Book() noexcept
        {
            for(unsigned shift=Pages::MaxShift;shift>=Pages::MinShift;--shift)
            {

            }
        }

    }

}

