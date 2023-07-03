//! \file

#ifndef Y_Type_Ints_Included
#define Y_Type_Ints_Included 1

#include "y/type/pick.hpp"

namespace Yttrium
{

    template <size_t N> struct SignedInt;

    //__________________________________________________________________________
    //
    //
    //! sizeof(SignedInt) = 1
    //
    //__________________________________________________________________________
    template <>
    struct SignedInt<1>
    {
        typedef int8_t    Type;                  //!< alias
        static const bool Signed  = true;        //!< alias
        static const Type Minimum = Type(0x80);  //!< alias
        static const Type Maximum = Type(0x7F);  //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(SignedInt) = 2
    //
    //__________________________________________________________________________
    template <>
    struct SignedInt<2>
    {
        typedef int16_t   Type;                   //!< alias
        static const bool Signed  = true;         //!< alias
        static const Type Minimum = Type(0x8000); //!< alias
        static const Type Maximum = Type(0x7FFF); //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(SignedInt) = 4
    //
    //__________________________________________________________________________
    template <>
    struct SignedInt<4>
    {
        typedef int32_t   Type;                       //!< alias
        static const bool Signed  = true;             //!< alias
        static const Type Minimum = Type(0x80000000); //!< alias
        static const Type Maximum = Type(0x7FFFFFFF); //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(SignedInt) = 8
    //
    //__________________________________________________________________________
    template <>
    struct SignedInt<8>
    {
        typedef int64_t   Type;                                //!< alias
        static const bool Signed  = true;                      //!< alias
        static const Type Minimum = Y_I64(0x8000000000000000); //!< alias
        static const Type Maximum = Y_I64(0x7FFFFFFFFFFFFFFF); //!< alias
    };


    template <size_t N> struct UnsignedInt;

    //__________________________________________________________________________
    //
    //
    //! sizeof(UnsignedInt) = 1
    //
    //__________________________________________________________________________
    template <>
    struct UnsignedInt<1>
    {
        typedef uint8_t   Type;                 //!< alias
        static const bool Signed  = false;      //!< alias
        static const Type Minimum = 0;          //!< alias
        static const Type Maximum = Type(0xFF); //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(UnsignedInt) = 2
    //
    //__________________________________________________________________________
    template <>
    struct UnsignedInt<2>
    {
        typedef uint16_t  Type;                     //!< alias
        static const bool Signed  = false;          //!< alias
        static const Type Minimum = 0;              //!< alias
        static const Type Maximum = Type(0xFFFF);   //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(UnsignedInt) = 4
    //
    //__________________________________________________________________________
    template <>
    struct UnsignedInt<4>
    {
        typedef uint32_t  Type;                       //!< alias
        static const bool Signed  = false;            //!< alias
        static const Type Minimum = 0;                //!< alias
        static const Type Maximum = Type(0xFFFFFFFF); //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! sizeof(UnsignedInt) = 8
    //
    //__________________________________________________________________________
    template <>
    struct UnsignedInt<8>
    {
        typedef uint64_t  Type;                                //!< alias
        static const bool Signed  = false;                     //!< alias
        static const Type Minimum = 0;                         //!< alias
        static const Type Maximum = Y_I64(0xFFFFFFFFFFFFFFFF); //!< alias
    };

    //__________________________________________________________________________
    //
    //
    //! static check is type is signed
    //
    //__________________________________________________________________________
    template <typename T>
    struct IsSigned {
        static const bool Value = (T)-1 < 0; //!< compile time detection
    };

    //__________________________________________________________________________
    //
    //
    //! Integer type mapping
    //
    //__________________________________________________________________________
    template <typename T>
    struct IntegerFor
    {
        static const size_t                           Bytes  = sizeof(T);          //!< alias
        static const bool                             Signed = IsSigned<T>::Value; //!< alias
        typedef SignedInt<Bytes>                      SInt;                        //!< signed version
        typedef UnsignedInt<Bytes>                    UInt;                        //!< unsigned version
        typedef typename Pick<Signed,SInt,UInt>::Type Info;                        //!< choose
        typedef typename Info::Type                   Type;                        //!< result

        static  const Type Minimum = Info::Minimum; //!< alias
        static  const Type Maximum = Info::Maximum; //!< alias

    };
    

}

#endif
