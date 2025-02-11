//! \file

#ifndef Y_Apex_CountNonZero_Included
#define Y_Apex_CountNonZero_Included 1

#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        //! default CountNonZero algorithm
        template <typename T>
        struct CountNonZero_
        {
            //! T is integral type
            static inline
            size_t In(const Readable<T> &arr) noexcept
            {
                size_t res = 0;
                for(size_t i=arr.size();i>0;--i)
                    if(0!=arr[i]) ++res;
                return res;
            }
        };

        //! specialized for Natural
        template <>
        struct CountNonZero_<Natural>
        {
            //! use bits to test
            static size_t In(const Readable<Natural> &arr) noexcept;
        };

        //! specialized for Integer
        template <>
        struct CountNonZero_<Integer>
        {
            //! use sign
            static size_t In(const Readable<Integer> &arr) noexcept;
        };

        //! specialized for Rational
        template <>
        struct CountNonZero_<Rational>
        {
            //! use numerator sign
            static size_t In(const Readable<Rational> &arr) noexcept;
        };


        //! dispatch counting algorithm
        template <typename T> inline
        size_t CountNonZero(const Readable<T> &arr) noexcept
        {
            return CountNonZero_<T>::In(arr);
        }
    }
}

#endif

