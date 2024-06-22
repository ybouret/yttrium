
//! \file

#ifndef Y_MKL_LAK_LU_Included
#define Y_MKL_LAK_LU_Included 1

#include "y/container/matrix.hpp"
#include "y/kemp/rational.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //______________________________________________________________________
        //
        //
        //
        //! Computing ranks of matrices
        //
        //
        //______________________________________________________________________
        struct Rank
        {
            //! compute rank of Q, destroyed
            static size_t Compute(Matrix<apq> &Q);

            //! copy compatible matrix and compute its rank
            template <typename T> static inline
            size_t Of(const Matrix<T> &M)
            {
                Matrix<apq> Q(CopyOf,M);
                return Compute(Q);
            }


        };

    }

}

#endif
