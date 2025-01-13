
#include "y/apex/block/data.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/type/utils.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Apex
    {

        DataBlock:: DataBlock(const unsigned _shift) :
        shift( Max(_shift,MinShift) ),
        entry( Memory::Archon::Acquire( Coerce(shift) ) ),
        bits(0),
        plan(Plan1),
        curr(0),
        dull(),
        range( One << shift )
        {
            assert( Memory::OutOfReach::Are0(entry,range) );
        }

        DataBlock:: ~DataBlock() noexcept
        {
            Memory::Archon::Release(entry,shift);
            Coerce(entry) = 0;
            Coerce(shift) = 0;
            Coerce(range) = 0;
            Coerce(curr)  = 0;
            Coerce(bits)  = 0;
            memset(dull,0,sizeof(dull));
        }
    }

}


