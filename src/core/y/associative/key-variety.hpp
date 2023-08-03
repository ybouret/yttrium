//! \file

#ifndef Y_Associative_Key_Variety_Included
#define Y_Associative_Key_Variety_Included 1

#include "y/type/traits.hpp"
#include "y/type/conversion.hpp"
#include "y/memory/buffer/ro.hpp"
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef typename TypeTraits<KEY>::MutableType MutableKey; //!< alias
            static const bool _IsMemoryBuffer = Y_Is_SuperSubClass(Memory::ReadOnlyBuffer,MutableKey); //!< inheritance
            static const bool _IsCharPointer_ = IsSameType<MutableKey,char *>::Value;                  //!< partial test
            static const bool _IsCharTableau_ = IsSameType<MutableKey,char []>::Value;                 //!< parital test
            static const bool _IsLegacyString = _IsCharPointer_ || _IsCharTableau_;                    //!< test for C-string
            static const bool _IsIntegralType = TypeTraits<MutableKey>::IsPrimitive;                   //!< other acceptabe

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
