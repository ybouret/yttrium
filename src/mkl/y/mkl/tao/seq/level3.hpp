//! \file

#ifndef Y_MKL_SeqTao3_Included
#define Y_MKL_SeqTao2_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {
         

            template <typename T, typename U, typename V, typename W> static inline
            void MMul(Matrix<T>       &target,
                      const Matrix<U> &lhs,
                      const Matrix<V> &rhs,
                      MultiAdd<W>     &xma)
            {
                assert(target.rows==lhs.rows);
                assert(target.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);
                const size_t ncom = lhs.cols;
                XAdd<W>     &xadd = xma.make(ncom);
                assert(xadd.isEmpty());
                assert(xadd.accepts(lhs.cols));

                for(size_t i=target.rows;i>0;--i)
                {
                    Writable<T>       &tgt_i = target[i];
                    const Readable<U> &lhs_i = lhs[i];
                    for(size_t j=target.cols;j>0;--j)
                    {
                        for(size_t k=ncom;k>0;--k)
                        {
                            const W p = lhs_i[k] * rhs[k][j];
                            xadd << p;
                        }
                        tgt_i[j] = xadd.sum();
                    }
                }

            }
        }

    }

}

#endif
