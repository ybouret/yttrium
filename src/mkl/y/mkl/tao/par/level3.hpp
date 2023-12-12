//! \file

#ifndef Y_MKL_ParTao3_Included
#define Y_MKL_ParTao3_Included 1


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
                template <typename T, typename U, typename V, typename W> static inline
                void MMul(Engine2D        &range,
                          Matrix<T>       &tgt,
                          const Matrix<U> &lhs,
                          const Matrix<V> &rhs)
                {
                    
                    if( range.isEmpty() ) return;

                    const size_t nrun = lhs.cols;
                    XAdd<W>     &xadd = range.xadd<W>();
                    assert(xadd.isEmpty());
                    assert(xadd.accepts(nrun));

                    for(size_t s=range->size;s>0;--s)
                    {
                        const Strip       &here  = range(s);
                        const size_t       i     = here.irow;
                        Writable<T>       &tgt_i = tgt[i];
                        const Readable<U> &lhs_i = lhs[i];
                        for(size_t j=here.icol,nc=here.ncol;nc>0;--nc,++j)
                        {
                            assert(xadd.isEmpty());
                            for(size_t k=nrun;k>0;--k)
                            {
                                const W p = lhs_i[k] * rhs[k][j];
                                xadd << p;
                            }
                            tgt_i[j] = xadd.sum();
                        }
                    }
                }
            }

            template <typename T, typename U, typename V, typename W> static inline
            void MMul(Matrix<T>       &tgt,
                      const Matrix<U> &lhs,
                      const Matrix<V> &rhs,
                      MultiAdd<W>     &xma,
                      Engine          &engine)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);

                engine.setup(tgt);                                    // parallel tiles of target
                engine.link2D(xma.make(engine.in2D.size(),lhs.cols)); // one xadd per tile

                try {
                    engine.in2D(Parallel::MMul<T,U,V,W>,tgt,lhs,rhs);
                    engine.free2D();
                }
                catch(...)
                {
                    engine.free2D();
                    throw;
                }


            }
        }

    }

}

#endif
