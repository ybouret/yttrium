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
                //! Mul on range
                template <typename TARGET, typename T, typename SOURCE, typename U> inline
                void Mul(Engine1D &range, TARGET &target, const Matrix<T> &M, SOURCE &source)
                {

                    if(range.length<=0) return;

                    XAdd<U>   &xadd = range.xadd<U>();
                    for(size_t row  = range.latest; row>=range.offset; --row)
                    {
                        assert(xadd.isEmpty());
                        target[row] = DotProduct<U>::Of_(M[row],source,xadd);
                    }
                }
            }

            //__________________________________________________________________
            //
            //! target = M*source
            //__________________________________________________________________
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
                engine.setup(M.rows);                      // process rows in parallel
                engine.in1D.attach(xma.make(para,M.cols)); // with help
                {
                    volatile Engine::Clean1D willClean(engine.in1D);
                    engine.in1D(Parallel::Mul<TARGET,T,SOURCE,U>,target,M,source);
                }
            }
        }

    }

}

#endif
