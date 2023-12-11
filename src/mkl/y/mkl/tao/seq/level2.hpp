
#ifndef Y_MKL_SeqTao2_Included
#define Y_MKL_SeqTao2_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/container/matrix.hpp"

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
                     Multifold<U>     &xmf)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                XAdd<U> &xadd = xmf.make(M.cols);
                for(size_t i=M.rows;i>0;--i)
                {
                    target[i] = DotProduct<U>::Of_(M[i],source,xadd);
                }
            }


        }

    }

}

#endif
