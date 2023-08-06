
//! \file

#ifndef Y_Type_XReal_Included
#define Y_Type_XReal_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    template <typename T>
    class XReal
    {
    public:
        const int exponent;
        const T   mantissa;

        XReal() noexcept;
        XReal(const T x);
        XReal(const XReal &) noexcept;
        XReal & operator=(const XReal &) noexcept;
        ~XReal() noexcept;
        
    };

}

#endif

