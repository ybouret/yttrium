
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

        //! Opaque uint8_t
        template <> struct Opaque<uint8_t>
        {
            static const uint8_t Value = 0xff; //!< value
        };

        //! Opaque uint16_t
        template <> struct Opaque<uint16_t>
        {
            static const uint16_t Value = 0xffff;//!< value
        };

        //! Opaque uint32_t
        template <> struct Opaque<uint32_t>
        {
            static const uint32_t Value = 0xffffffff;//!< value
        };

        //! Opaque uint32_t
        template <> struct Opaque<uint64_t>
        {
            static const uint64_t Value = Y_U64(0xffffffffffffffff);//!< value
        };

        //! Opaque float
        template <> struct Opaque<float>
        {
            static const float Value; //!< value
        };

        //! Opaque double
        template <> struct Opaque<double>
        {
            static const double Value; //!< value
        };

        //! Opaque long double
        template <> struct Opaque<long double>
        {
            static const long double Value; //!< value
        };

    }

}

#endif
