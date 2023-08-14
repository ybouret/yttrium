#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{
    Little_Endian_Address:: ~Little_Endian_Address() noexcept {}

    Little_Endian_Address:: Little_Endian_Address(const Little_Endian_Address &other) noexcept : Little_Endian_Key( static_cast<const Little_Endian_Key&>(other) ) {}


    void * Little_Endian_Address:: fetch() const noexcept
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


