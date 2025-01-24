
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

        void DataBlock:: set_(const size_t ibit) noexcept
        {
            assert(Plan1==plan);
            assert(ibit<8*range);
            static_cast<uint8_t *>(entry)[ibit>>3] |= _bit[ibit&7];
        }

        bool DataBlock:: get_(const size_t ibit) const noexcept
        {
            assert(Plan1==plan);
            assert(ibit<8*range);
            return 0 != (static_cast<const uint8_t *>(entry)[ibit>>3] & _bit[ibit&7] );
        }
        
        void  DataBlock:: clr_(const size_t ibit) noexcept
        {
            static const uint8_t _msk[8] = { 255-1,255-2,255-4,255-8,255-16,255-32,255-64,255-128 };
            assert(Plan1==plan);
            assert(ibit<8*range);
            static_cast<uint8_t *>(entry)[ibit>>3] &= _msk[ibit&7];
        }



    }

}


