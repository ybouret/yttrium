#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{
    LittleEndianAddress:: ~LittleEndianAddress() noexcept {}

    LittleEndianAddress:: LittleEndianAddress(const LittleEndianAddress &other) noexcept : LittleEndianKey( static_cast<const LittleEndianKey&>(other) ) {}


    LittleEndianAddress:: LittleEndianAddress(const CopyOf_ &, const void *addr) noexcept :
    LittleEndianKey(addr)
    {
    }
    

    void * LittleEndianAddress:: fetch() const noexcept
    {
        assert(size==sizeof(void*));

        union
        {
            UnsignedInt<sizeof(void*)>::Type code;
            void                            *addr;
        }
        alias = { get<sizeof(void*)>() };

        return alias.addr;
    }

}


