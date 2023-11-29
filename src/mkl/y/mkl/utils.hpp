//! file

#ifndef Y_MKL_Utils_Included
#define Y_MKL_Utils_Included 1

#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace MKL
    {
        struct OutOfReach
        {
            template <typename T> static T Add(const T &lhs, const T &rhs);
            template <typename T> static T Sub(const T &lhs, const T &rhs);

        };


        template <typename T> inline
        bool IsUnchanged(const T &value, const T &delta)
        {
            typedef typename ScalarFor<T>::Type scalar;
            const T      added = OutOfReach::Add(value,delta);
            const scalar lhs   = Fabs<T>::Of(value);
            const scalar rhs   = Fabs<T>::Of(added);
            const scalar dif   = lhs-rhs;
            const scalar _0_   = 0;
            return Fabs<scalar>::Of(dif) <= _0_;
        }

    }
}

#endif

