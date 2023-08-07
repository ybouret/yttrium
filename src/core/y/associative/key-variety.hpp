//! \file

#ifndef Y_Associative_Key_Variety_Included
#define Y_Associative_Key_Variety_Included 1

#include "y/type/traits.hpp"
#include "y/type/conversion.hpp"
#include "y/memory/buffer/ro.hpp"
#include "y/mkl/v3d.hpp"
#include "y/mkl/complex.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! classification of keys
    //
    //
    //__________________________________________________________________________
    struct KeyVariety
    {
        //______________________________________________________________________
        //
        //
        //! Possible outcomes
        //
        //______________________________________________________________________
        enum Type
        {
            IsNotAvailable,
            IsMemoryBuffer,
            IsIntegralType,
            IsLegacyString
        };

        //______________________________________________________________________
        //
        //! helper
        //______________________________________________________________________
        static const char *Type2Text(const Type) noexcept;

        //______________________________________________________________________
        //
        //
        //! Making a sieve of possibilites
        //
        //______________________________________________________________________
        template <typename KEY>
        struct Cull
        {
        private:
            template <class U>    struct IsCompoundTrait               { enum { Value = false }; };
            template <typename Z> struct IsCompoundTrait< V2D<Z> >     { enum { Value = true };  };
            template <typename Z> struct IsCompoundTrait< V3D<Z> >     { enum { Value = true };  };
            template <typename Z> struct IsCompoundTrait< Complex<Z> > { enum { Value = true };  };

        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename TypeTraits<KEY>::MutableType MutableKey; //!< alias
            enum { IsCompound = IsCompoundTrait<MutableKey>::Value };
            static const bool _IsMemoryBuffer = Y_Is_SuperSubClass(Memory::ReadOnlyBuffer,MutableKey);   //!< inheritance
            static const bool _IsCharPointer_ = IsSameType<MutableKey,char *>::Value;                    //!< partial test
            static const bool _IsCharTableau_ = TypeTraits<MutableKey>::template IsArrayOf<char>::Value; //!< partial test
            static const bool _IsLegacyString = _IsCharPointer_ || _IsCharTableau_;                      //!< test for C-string
            static const bool _IsIntegralType = TypeTraits<MutableKey>::IsPrimitive || IsCompound;       //!< other acceptabe

            //__________________________________________________________________
            //
            //
            //! evaluate type with precedence of LegacyString over IntegralType
            //
            //__________________________________________________________________
            static const Type Kind =
            (   _IsLegacyString ? IsLegacyString :
             (  _IsIntegralType ? IsIntegralType :
              ( _IsMemoryBuffer ? IsMemoryBuffer : IsNotAvailable)));
        };


        typedef Int2Type<IsIntegralType> IntegralType; //!< alias
        typedef Int2Type<IsLegacyString> LegacyString; //!< alias
        typedef Int2Type<IsMemoryBuffer> MemoryBuffer; //!< alias


    };

}

#endif
