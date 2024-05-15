
//! \file

#ifndef Y_MKL_OrthoSpace_Included
#define Y_MKL_OrthoSpace_Included 1

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
             - may return an empty matrix
             */
            static bool Build(Matrix<apz> &Q, const Matrix<apz> &P);

            //! make from any compatible matrix P
            template <typename T>
            static bool Make(Matrix<apz> &Q,const Matrix<T> &P)
            {
                Matrix<apz> PP(CopyOf,P);
                return Build(Q,PP);
            }

        };

    }

}


#endif
