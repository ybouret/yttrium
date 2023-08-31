

//! \file

#ifndef Y_Apex_Ortho_Space_Included
#define Y_Apex_Ortho_Space_Included 1

#include "y/apex/integer.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{

    namespace Apex
    {

        namespace Ortho
        {

            struct Space
            {
                static void Build(const Matrix<apz> &P);
                
            };

        }

    }

}

#endif
