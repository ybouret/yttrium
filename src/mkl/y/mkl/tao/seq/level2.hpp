//! \file

#ifndef Y_MKL_SeqTao2_Included
#define Y_MKL_SeqTao2_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/ops.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {


            namespace Cog
            {
                //! proc(target,M*source)
                template <
                typename TARGET,
                typename T,
                typename SOURCE,
                typename U,
                typename PROC> inline
                void Mul(TARGET &          target,
                         const Matrix<T>  &M,
                         SOURCE           &source,
                         MultiAdd<U>      &xma,
                         PROC             &proc)
                {
                    assert( target.size() == M.rows );
                    assert( source.size() == M.cols );
                    XAdd<U> &xadd = xma.make(M.cols);
                    for(size_t i=M.rows;i>0;--i)
                    {
                        const U rhs = DotProduct<U>::Of_(M[i],source,xadd);
                        proc(target[i],rhs);
                    }
                }
            }


            //! target = M*source
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void Mul(TARGET &          target,
                     const Matrix<T>  &M,
                     SOURCE           &source,
                     MultiAdd<U>      &xma)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                Cog::Mul(target,M,source,xma,Ops<typename TARGET::Type,U>::Set);
            }

            //! target += M*source
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void MulAdd(TARGET &          target,
                        const Matrix<T>  &M,
                        SOURCE           &source,
                        MultiAdd<U>      &xma)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                Cog::Mul(target,M,source,xma,Ops<typename TARGET::Type,U>::Add);
            }

            //! target -= M*source
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void MulSub(TARGET &          target,
                        const Matrix<T>  &M,
                        SOURCE           &source,
                        MultiAdd<U>      &xma)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                Cog::Mul(target,M,source,xma,Ops<typename TARGET::Type,U>::Sub);
            }

            

        }

    }

}

#endif
