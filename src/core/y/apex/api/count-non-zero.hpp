//! \file

#ifndef Y_Apex_CountNonZero_Included
#define Y_Apex_CountNonZero_Included 1

#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace Apex
    {

        template <typename T>
        struct CountNonZero_
        {
            static inline
            size_t In(const Readable<T> &arr) noexcept
            {
                size_t res = 0;
                for(size_t i=arr.size();i>0;--i)
                    if(0!=arr[i]) ++res;
                return res;
            }
        };


        template <>
        struct CountNonZero_<Natural>
        {
            static inline
            size_t In(const Readable<Natural> &arr) noexcept
            {
                size_t res = 0;
                for(size_t i=arr.size();i>0;--i)
                    if(arr[i]->bits>0) ++res;
                return res;
            }
        };

        template <typename T> inline
        size_t CountNonZero(const Readable<T> &arr) noexcept
        {
            return CountNonZero_<T>::In(arr);
        }
    }
}

#endif

