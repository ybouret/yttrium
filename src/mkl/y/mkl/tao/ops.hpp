
//! \file

#ifndef Y_MKL_Tao_Ops_Included
#define Y_MKL_Tao_Ops_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {
        
        namespace Tao
        {
            
            template <typename LHS, typename RHS>
            struct Ops
            {
                static inline void Set(LHS &lhs, const RHS &rhs) { lhs = rhs; }
            };
            
        }
        
    }
}

#endif
