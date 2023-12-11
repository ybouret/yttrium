//! \file

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

            namespace Parallel
            {
                template <typename TARGET, typename T, typename SOURCE, typename U> inline
                void Mul(Engine1D &range, TARGET &target, const Matrix<T> &M, SOURCE &source)
                {
                    assert(0!=range.anonymous);

                    XAdd<U>   &xadd = *static_cast< XAdd<U> *>(range.anonymous);
                    size_t     row  = range.latest;
                    for(size_t num  = range.length;num>0;--num,--row)
                    {
                        target[row] = DotProduct<U>::Of_(M[row],source,xadd);
                    }
                }
            }

            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void Mul(TARGET &          target,
                     const Matrix<T>  &M,
                     SOURCE           &source,
                     MultiAdd<U>      &xma,
                     Engine           &engine)
            {
                assert( target.size() == M.rows );
                assert( source.size() == M.cols );
                const size_t   para   = engine.in1D.size();
                engine.setup(M.rows);                   // process rows in parallel
                engine.link1D(xma.make(para,M.cols));   // with help

                try {
                    engine.in1D(Parallel::Mul<TARGET,T,SOURCE,U>,target,M,source);
                    engine.free1D();
                }
                catch(...)
                {
                    engine.free1D();
                    throw;
                }



            }
        }

    }

}

#endif
