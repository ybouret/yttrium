
//! \file

#ifndef Y_MKL_Tao_Ops_Included
#define Y_MKL_Tao_Ops_Included 1


#include "y/mkl/tao/transmogrify.hpp"

namespace Yttrium
{
    namespace MKL
    {
        
        namespace Tao
        {

            //__________________________________________________________________
            //
            //! wrapper for parallel operations
            //__________________________________________________________________
            template <typename LHS, typename RHS>
            struct Ops
            {
                static inline void Set(LHS &lhs, const RHS &rhs) { lhs  = rhs; } //!< Set
                static inline void Add(LHS &lhs, const RHS &rhs) { lhs += rhs; } //!< Add
                static inline void Sub(LHS &lhs, const RHS &rhs) { lhs -= rhs; } //!< Sub
            };

        }
        
    }
}

#endif
