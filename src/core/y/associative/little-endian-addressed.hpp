
//! \file

#ifndef Y_Associative_LE_Addressed_Included
#define Y_Associative_LE_Addressed_Included 1

#include "y/associative/little-endian-address.hpp"
#include "y/type/conversion.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{

    class LittleEndianAddressed
    {
    public:
        explicit LittleEndianAddressed() noexcept;
        virtual ~LittleEndianAddressed() noexcept;

        const LittleEndianAddress leak;

        template <typename T> inline
        T & as() noexcept
        {
            static const bool Inherited = Y_Is_SuperSubClass(LittleEndianAddressed,T);
            Y_STATIC_CHECK(Inherited,BadCast);
            return *static_cast<T*>( leak.fetch() );
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(LittleEndianAddressed);
    };
}

#endif

