
//! \file

#ifndef Y_MKL_Jacobi_Included
#define Y_MKL_Jacobi_Included 1


#include "y/container/matrix.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Eigen
        {

            template <typename T>
            void Balance(Matrix<T> &a);

        }

    }

}

#endif
