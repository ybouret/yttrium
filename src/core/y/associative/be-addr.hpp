
//! \file

#ifndef Y_Associative_BE_Addr_Included
#define Y_Associative_BE_Addr_Included 1

#include "y/associative/be-key.hpp"

namespace Yttrium
{


    class BE_Addr : public BE_Key
    {
    public:
        virtual ~BE_Addr() noexcept;
        
    private:
        Y_DISABLE_ASSIGN(BE_Addr);
    };

}

#endif
