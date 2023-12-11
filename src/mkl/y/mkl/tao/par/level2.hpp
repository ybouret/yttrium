

#ifndef Y_MKL_ParTao2_Included
#define Y_MKL_ParTao2_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/tao/engine.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {

            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void Mul(TARGET &          target,
                     const Matrix<T>  &M,
                     SOURCE           &source,
                     Antelope::Add<U> &xadd,
                     Engine           &engine)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                //for(size_t i=M.rows;i>0;--i)                     target[i] = DotProduct<U>::Of(M[i],source,xadd);
            }
        }

    }

}

#endif
