
//! \file

#ifndef Y_MKL_LAK_OrthoSpace_Included
#define Y_MKL_LAK_OrthoSpace_Included 1

#include "y/container/matrix.hpp"
#include "y/apex/integer.hpp"

namespace Yttrium
{
    namespace MKL
    {
        
        struct OrthoSpace
        {
            static bool Build(const Matrix<apz> &P);
        };

    }

}


#endif
