
//! \file

#ifndef Y_Associative_LE_Addressed_Included
#define Y_Associative_LE_Addressed_Included 1

#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{

    class LittleEndianAddressed
    {
    public:
        explicit LittleEndianAddressed() noexcept;
        virtual ~LittleEndianAddressed() noexcept;

        const LittleEndianAddress leak;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LittleEndianAddressed);
    };
}

#endif

