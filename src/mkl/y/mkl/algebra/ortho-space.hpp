
//! \file

#ifndef Y_MKL_LAK_OrthoSpace_Included
#define Y_MKL_LAK_OrthoSpace_Included 1

#include "y/container/matrix.hpp"
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //! Orthogonal Space computation
        struct OrthoSpace
        {
            //! compute orthogonal family of vectors
            /**
             - P(p,d),    free family of (row) vectors
             - Q(q<=d,d), univocal set of row
             - Q*P' = 0
             */
            static bool Build(Matrix<apz> &Q, const Matrix<apz> &P);

            template <typename T>
            static bool Build(Matrix<apz> &Q, const CopyOf_ &_, const Matrix<T> &P)
            {
                Matrix<apz> PP(_,P);
                return Build(Q,PP);
            }

        };

    }

}


#endif
