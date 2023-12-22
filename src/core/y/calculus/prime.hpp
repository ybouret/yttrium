//! \file

#ifndef Y_Calculus_Prime_Included
#define Y_Calculus_Prime_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //! basic function for primality
        //
        //______________________________________________________________________
        struct Prime
        {
            //__________________________________________________________________
            //
            //
            //! generic division-trial algorithm
            //
            //__________________________________________________________________
            template <typename T> static inline
            bool Check(const T n)
            {
                if( n == 2 || n == 3)
                    return true;

                if( n <= 1 || (n % 2) == 0 || (n % 3) == 0)
                    return false;

                for(T i = 5; i*i <= n; i += 6)
                {
                    if ( (n%i) == 0 || (n % (i + 2)) == 0)
                        return false;
                }

                return true;
            }

            //__________________________________________________________________
            //
            //
            // Precomputed tables
            //
            //__________________________________________________________________
            static const size_t   N8 = 54; //!< primes with less than 8 bits
            static const uint8_t  P8[N8];  //!< table 2:251

            static const size_t   N16 = 6488; //!< primes with 9 to 16 bits
            static const uint16_t P16[N16];   //!< table 257:65521

            //__________________________________________________________________
            //
            //
            // Access to all less than 16 bits
            //
            //__________________________________________________________________
            static const size_t   Pi8  = N8;
            static const size_t   Pi16 = N8+N16; //!< number of primes < 2^16
            static unsigned       Get(const size_t i) noexcept; //!< in [1..Pi16]

            //__________________________________________________________________
            //
            //
            //! Precomputed Pi(2^n)
            //
            //__________________________________________________________________
            static const uint32_t   Pi2ToThe[37]; 

        };

    }

    //! prime for 64bits unsigned
    struct Prime64
    {
        static bool     Is_(const uint64_t n) noexcept; //!< check is prime
        static uint64_t Next(uint64_t n)      noexcept; //!< get next prime
    };


}

#endif
