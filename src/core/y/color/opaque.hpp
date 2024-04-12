
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
            constexpr static  const uint8_t Value = 0xff;
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

        template <> struct Opaque<double>
        {
            static constexpr double Value = 1.0 ;
        };

        template <> struct Opaque<long double>
        {
            static constexpr long double Value = 1.0l ;
        };

    }

}

#endif
