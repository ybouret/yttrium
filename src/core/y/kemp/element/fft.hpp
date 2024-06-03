//! \file

#ifndef Y_Kemp_Element_FFT_Included
#define Y_Kemp_Element_FFT_Included 1

#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        struct FFT_Multiplication
        {

            static Element * Get(const Assembly<uint8_t> &lhs,
                                 const Assembly<uint8_t> &rhs,
                                 uint64_t * const         tmx);

            static inline Element * Result( Element &lhs, Element &rhs )     
            {
                return Get(lhs.get<uint8_t>(), rhs.get<uint8_t>(),0);
            }

            static inline Element * ResTMX( Element &lhs, Element &rhs, uint64_t &tmx)
            {
                return Get(lhs.get<uint8_t>(), rhs.get<uint8_t>(),&tmx);
            }

            static inline Element * ResL64( uint64_t lhs,  Element &rhs)
            {
                const Assembly<uint8_t> L(lhs);
                return Get(L,rhs.get<uint8_t>(),0);
            }

            static inline Element * ResR64( Element &lhs, uint64_t rhs )
            {
                const Assembly<uint8_t> R(rhs);
                return Get(lhs.get<uint8_t>(),R,0);
            }

        };

    }

}

#endif

