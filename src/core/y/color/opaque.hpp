
//! \file

#ifndef Y_Color_Opaque_Included
#define Y_Color_Opaque_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Color
    {
        template <typename T>
        struct Opaque;


        template <> struct Opaque<uint8_t>
        {
            static constexpr uint8_t Value = 0xff;
        };

        template <> struct Opaque<uint16_t>
        {
            static constexpr uint16_t Value = 0xffff;
        };

        template <> struct Opaque<uint32_t>
        {
            static constexpr uint32_t Value = 0xffffffff;
        };

        template <> struct Opaque<float>
        {
            static constexpr float Value = 1.0f ;
        };

    }

}

#endif
