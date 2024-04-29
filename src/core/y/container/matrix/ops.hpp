//! \file

#ifndef Y_Matrix_Ops_Included
#define Y_Matrix_Ops_Included 1

#include "y/container/matrix.hpp"
#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{

    struct MatrixOps
    {


        template <typename TARGET,typename T, typename SOURCE, typename U> static inline
        TARGET & Mul(TARGET &target, const Matrix<T> &M, SOURCE &source, MKL::Antelope::Add<U> &xadd)
        {
            assert(target.size()==M.rows);
            assert(source.size()==M.cols);
            const size_t nc = M.cols;
            xadd.make(nc); assert( xadd.isEmpty() );
            for(size_t i=M.rows;i>0;--i)
            {
                assert( xadd.isEmpty() );
                const Readable<T> &Mi = M[i];
                for(size_t j=nc;j>0;--j) 
                {
                    const T res = Mi[j] * source[j];
                    xadd << res;
                }
                target[i] = xadd.sum();
            }

            return target;
        }
    };

}

#endif

