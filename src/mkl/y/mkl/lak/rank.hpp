
//! \file

#ifndef Y_MKL_LAK_LU_Included
#define Y_MKL_LAK_LU_Included 1

#include "y/container/matrix.hpp"
#include "y/apex/rational.hpp"

namespace Yttrium
{
    namespace MKL
    {

        struct Rank
        {
            static size_t Compute(Matrix<apq> &M);
            
        };

    }

}

#endif
