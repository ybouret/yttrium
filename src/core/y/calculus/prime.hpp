//! \file

#ifndef Y_Calculus_Prime_Included
#define Y_Calculus_Prime_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        //! basic function for primality
        struct Prime
        {
            //! generic is_prime function
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
        };

    }

    //! prime for 64bits unsigned
    struct Prime64
    {
        static bool     Check(const uint64_t n) noexcept; //!< check is prime
        static uint64_t Next(uint64_t n)        noexcept; //!< get next prime
    };


}

#endif
