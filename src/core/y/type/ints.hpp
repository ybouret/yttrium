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
        static const Type Maxi1   = Type(0xFF); //!< alias
        static const Type Mask1   = Type(0xFF); //!< alias

        //! compute bytes for one byte
        static inline unsigned BytesFor(const Type x) noexcept
        {
            return x>0 ? 1 : 0;
        }

        //! shift right 8 bits
        static inline Type SHR8(const Type) noexcept { return 0; }

        //! shift left 8 bits
        static inline Type SHL8(const Type) noexcept { return 0; }

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
        typedef uint16_t  Type;                   //!< alias
        static const bool Signed  = false;        //!< alias
        static const Type Minimum = 0;            //!< alias
        static const Type Maximum = Type(0xFFFF); //!< alias
        static const Type Mask2   = Type(0xFF00); //!< aliss
        static const Type Mask1   = Type(0x00FF); //!< alias
        static const Type Maxi1   = Type(0xFF);   //!< alias
        static const Type Maxi2   = Type(0xFFFF); //!< alias

        //! compute bytes for a word
        static inline unsigned BytesFor(const Type x) noexcept
        {
            if(0 != (x&Mask2) ) return 2;
            if(0 != (x&Mask1) ) return 1;
            return 0;
        }

        //! shift right 8 bits
        static inline Type SHR8(const Type x) noexcept { return x>>8; }

        //! shift left 8 bits
        static inline Type SHL8(const Type x) noexcept { return x<<8; }
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
        static const Type Mask4   = Type(0xFF000000); //!< alias
        static const Type Mask3   = Type(0x00FF0000); //!< alias
        static const Type Mask2   = Type(0x0000FF00); //!< alias
        static const Type Mask1   = Type(0x000000FF); //!< alias

        static const Type Maxi4   = Type(0xFFFFFFFF); //!< alias
        static const Type Maxi3   = Type(0x00FFFFFF); //!< alias
        static const Type Maxi2   = Type(0x0000FFFF); //!< alias
        static const Type Maxi1   = Type(0x000000FF); //!< alias

        //! compute bytes for a dword
        static inline unsigned BytesFor(const Type x) noexcept
        {
            if(0 != (x&Mask4) ) return 4;
            if(0 != (x&Mask3) ) return 3;
            if(0 != (x&Mask2) ) return 2;
            if(0 != (x&Mask1) ) return 1;
            return 0;
        }

        //! shift right 8 bits
        static inline Type SHR8(const Type x) noexcept { return x>>8; }

        //! shift left 8 bits
        static inline Type SHL8(const Type x) noexcept { return x<<8; }
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
        static const Type Maximum = Y_U64(0xFFFFFFFFFFFFFFFF); //!< alias
        static const Type Mask8   = Y_U64(0xFF00000000000000); //!< alias
        static const Type Mask7   = Y_U64(0x00FF000000000000); //!< alias
        static const Type Mask6   = Y_U64(0x0000FF0000000000); //!< alias
        static const Type Mask5   = Y_U64(0x000000FF00000000); //!< alias
        static const Type Mask4   = Y_U64(0x00000000FF000000); //!< alias
        static const Type Mask3   = Y_U64(0x0000000000FF0000); //!< alias
        static const Type Mask2   = Y_U64(0x000000000000FF00); //!< alias
        static const Type Mask1   = Y_U64(0x00000000000000FF); //!< alias

        static const Type Maxi8   = Y_U64(0xFFFFFFFFFFFFFFFF); //!< alias
        static const Type Maxi7   = Y_U64(0x00FFFFFFFFFFFFFF); //!< alias
        static const Type Maxi6   = Y_U64(0x0000FFFFFFFFFFFF); //!< alias
        static const Type Maxi5   = Y_U64(0x000000FFFFFFFFFF); //!< alias
        static const Type Maxi4   = Y_U64(0x00000000FFFFFFFF); //!< alias
        static const Type Maxi3   = Y_U64(0x0000000000FFFFFF); //!< alias
        static const Type Maxi2   = Y_U64(0x000000000000FFFF); //!< alias
        static const Type Maxi1   = Y_U64(0x00000000000000FF); //!< alias

        //! compute bytes for a qword
        static inline unsigned BytesFor(const Type x) noexcept
        {
            if(0 != (x&Mask8) ) return 8;
            if(0 != (x&Mask7) ) return 7;
            if(0 != (x&Mask6) ) return 6;
            if(0 != (x&Mask5) ) return 5;
            if(0 != (x&Mask4) ) return 4;
            if(0 != (x&Mask3) ) return 3;
            if(0 != (x&Mask2) ) return 2;
            if(0 != (x&Mask1) ) return 1;
            return 0;
        }

        //! shift right 8 bits
        static inline Type SHR8(const Type x) noexcept { return x>>8; }

        //! shift left 8 bits
        static inline Type SHL8(const Type x) noexcept { return x<<8; }
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

    //! computing bytes required for an integral type
    template <typename T>
    static inline unsigned RequiredBytesFor(const T x) noexcept
    {
        return IntegerFor<T>::UInt::BytesFor(x);
    }


}

#endif
