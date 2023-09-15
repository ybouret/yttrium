//! \file

#ifndef Y_Chemical_Algebra_Included
#define Y_Chemical_Algebra_Included 1

#include "y/container/matrix.hpp"
#include "y/container/cxx/array.hpp"
#include "y/apex/integer.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        struct Algebraic
        {

            static void Compute(Matrix<unsigned> &Q, const Matrix<int> &Nu);

        };

    }

}

#endif

