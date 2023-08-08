
//! \file

#ifndef Y_Type_FourCC_Included
#define Y_Type_FourCC_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

#define Y_FourCC0(a) (static_cast<uint32_t>(a))
#define Y_FourCC1(a) (Y_FourCC0(a) <<  8)
#define Y_FourCC2(a) (Y_FourCC0(a) << 16)
#define Y_FourCC3(a) (Y_FourCC0(a) << 24)

#define Y_FOURCC(a,b,c,d) (Y_FourCC3(a)|Y_FourCC2(b)|Y_FourCC1(c)|Y_FourCC0(d))

    struct FourCC
    {
        static const char * ToText(const uint32_t uuid) noexcept;
    };

}

#endif
