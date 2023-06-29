//! \file

#ifndef Y_Type_Ints_Included
#define Y_Type_Ints_Included 1

#include "y/type/pick.hpp"

namespace Yttrium
{

    template <size_t N> struct SignedInt;

    template <>
    struct SignedInt<1>
    {
        typedef int8_t    Type;
        static const bool Signed  = true;
        static const Type Minimum = Type(0x80);
        static const Type Maximum = Type(0x7F);
    };

    template <>
    struct SignedInt<2>
    {
        typedef int16_t   Type;
        static const bool Signed  = true;
        static const Type Minimum = Type(0x8000);
        static const Type Maximum = Type(0x7FFF);
    };

    template <>
    struct SignedInt<4>
    {
        typedef int32_t   Type;
        static const bool Signed  = true;
        static const Type Minimum = Type(0x80000000);
        static const Type Maximum = Type(0x7FFFFFFF);
    };

    template <>
    struct SignedInt<8>
    {
        typedef int64_t   Type;
        static const bool Signed  = true;
        static const Type Minimum = Y_I64(0x8000000000000000);
        static const Type Maximum = Y_I64(0x7FFFFFFFFFFFFFFF);
    };


    template <size_t N> struct UnsignedInt;

    template <>
    struct UnsignedInt<1>
    {
        typedef uint8_t   Type;
        static const bool Signed  = false;
        static const Type Minimum = 0;
        static const Type Maximum = Type(0xFF);
    };

    template <>
    struct UnsignedInt<2>
    {
        typedef uint16_t  Type;
        static const bool Signed  = false;
        static const Type Minimum = 0;
        static const Type Maximum = Type(0xFFFF);
    };


    template <>
    struct UnsignedInt<4>
    {
        typedef uint32_t  Type;
        static const bool Signed  = false;
        static const Type Minimum = 0;
        static const Type Maximum = Type(0xFFFFFFFF);
    };

    template <>
    struct UnsignedInt<8>
    {
        typedef uint64_t  Type;
        static const bool Signed  = false;
        static const Type Minimum = 0;
        static const Type Maximum = Y_I64(0xFFFFFFFFFFFFFFFF);
    };

    template <typename T>
    struct IsSigned {
        static const bool Value = (T)-1 < 0; //!< compile time detection
    };

    template <typename T>
    struct IntegerFor
    {
        static const size_t                           Bytes  = sizeof(T);
        static const bool                             Signed = IsSigned<T>::Value;
        typedef SignedInt<Bytes>                      SInt;
        typedef UnsignedInt<Bytes>                    UInt;
        typedef typename Pick<Signed,SInt,UInt>::Type Info;
        typedef typename Info::Type                   Type;

        static  const Type Minimum = Info::Minimum;
        static  const Type Maximum = Info::Maximum;

    };
    

}

#endif
