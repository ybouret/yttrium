#include "y/associative/be-addr.hpp"

namespace Yttrium
{
    BE_Addr:: ~BE_Addr() noexcept {}

    BE_Addr:: BE_Addr(const BE_Addr &other) noexcept : BE_Key( static_cast<const BE_Key&>(other) ) {}


    const void * BE_Addr:: fetch() const noexcept
    {
        assert(size==sizeof(void*));

        
        return 0;
    }

}


