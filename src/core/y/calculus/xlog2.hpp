
//! \file

#ifndef Y_Calculus_XLog2_Included
#define Y_Calculus_XLog2_Included 1

#include "y/type/ints.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! exact log2 algorithm
        //
        //______________________________________________________________________
        template <typename T>
        struct ExactLog2
        {
            static const unsigned SIZE = sizeof(T); //!< alias
            static const unsigned BITS = 8 * SIZE;  //!< alias
            static const T        XTAB[BITS];       //!< XTAB[i] = 2^i

            //! get exact log2
            static inline unsigned Of(const T n) noexcept
            {
                for(unsigned i=0;i<BITS;++i)
                {
                    if(n==XTAB[i]) return i;
                }
                return BITS; //!< invalid
            }
        };
        
#if !defined(_MSC_VER)
        template <> const uint8_t  ExactLog2<uint8_t>::  XTAB[]; //!< decl
        template <> const uint16_t ExactLog2<uint16_t>:: XTAB[]; //!< decl
        template <> const uint32_t ExactLog2<uint32_t>:: XTAB[]; //!< decl
        template <> const uint64_t ExactLog2<uint64_t>:: XTAB[]; //!< decl
#endif

    }

    //__________________________________________________________________________
    //
    //
    //! wrapper to get exact log2 of an unsigned integer
    //
    //__________________________________________________________________________
    template <typename T> static inline
    unsigned ExactLog2(const T n) noexcept
    {
        return Core::ExactLog2< typename UnsignedInt<sizeof(T)>::Type >::Of(n);
    }

}


#endif
