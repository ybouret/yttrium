
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


        namespace
        {
            static const uint8_t _bit[8] = { 1,2,4,8,16,32,64,128 };
        }

        void DataBlock:: set(const size_t ibit) noexcept
        {
            assert(Plan1==plan);
            assert(ibit<bits);
            static_cast<uint8_t *>(entry)[ibit>>3] |= _bit[ibit&7];
        }

        bool DataBlock:: get(const size_t ibit) const noexcept
        {
            assert(Plan1==plan);
            assert(ibit<bits);
            return 0 != (static_cast<const uint8_t *>(entry)[ibit>>3] & _bit[ibit&7] );
        }
        

        


    }

}


