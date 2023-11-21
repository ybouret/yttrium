//! \file

#ifndef Y_Calculus_Gray_Included
#define Y_Calculus_Gray_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Gray Code managements
    //
    //
    //__________________________________________________________________________
    struct Gray
    {
        //______________________________________________________________________
        //
        //! Encode
        //______________________________________________________________________
        template <typename T> static inline
        T Encode(const T n) noexcept
        {
            return n^(n>>1);
        }

        //______________________________________________________________________
        //
        //! Decode
        //______________________________________________________________________
        template <typename T> static inline
        T Decode(const T n) noexcept
        {
            static const unsigned WordBits = sizeof(T)*8;
            static const unsigned StopBits = WordBits/2;
            unsigned ish = 1;
            T        ans = n;
            for(;;)
            {
                const T idiv = ans>>ish;
                ans ^= idiv;
                if(idiv <= 1 || ish == StopBits) return ans;
                ish <<= 1;
            }
        }

    };
}

#endif
