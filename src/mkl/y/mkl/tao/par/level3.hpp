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
                    {
                        Y_LOCK(range.sync);
                        std::cerr << "computing on " << range << std::endl;
                    }
                    
#if 0
                    for(size_t k=range.size;k>0;--k)
                    {

                    }
#endif

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

                engine.setup(tgt); // parallel tiles of target

                engine.in2D(Parallel::MMul<T,U,V,W>,tgt,lhs,rhs);


            }
        }

    }

}

#endif
