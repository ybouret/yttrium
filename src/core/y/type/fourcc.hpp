
//! \file

#ifndef Y_Type_FourCC_Included
#define Y_Type_FourCC_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

#define Y_FourCC0(a) (static_cast<uint32_t>(a))   //!< byte 0/4
#define Y_FourCC1(a) (Y_FourCC0(a) <<  8)         //!< byte 1/4
#define Y_FourCC2(a) (Y_FourCC0(a) << 16)         //!< byte 2/4
#define Y_FourCC3(a) (Y_FourCC0(a) << 24)         //!< byte 3/4

    //! make 32bits flag
#define Y_FOURCC(a,b,c,d) (Y_FourCC3(d)|Y_FourCC2(c)|Y_FourCC1(b)|Y_FourCC0(a))

    //! helper to Four Characters Codes
    struct FourCC
    {
        //! unsafe uuid to text conversion
        static const char * ToText(const uint32_t uuid) noexcept;
    };

}

#endif
