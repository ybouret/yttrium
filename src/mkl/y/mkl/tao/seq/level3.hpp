//! \file

#ifndef Y_MKL_SeqTao3_Included
#define Y_MKL_SeqTao3_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/container/matrix.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {

            //__________________________________________________________________
            //
            //
            //
            //! Matrix Multiplication
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MMul(Matrix<T>       &tgt,
                      const Matrix<U> &lhs,
                      const Matrix<V> &rhs,
                      MultiAdd<W>     &xma)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);
                const size_t ncol = tgt.cols;
                const size_t nrun = lhs.cols;
                XAdd<W>     &xadd = xma.make(nrun);

                assert(xadd.isEmpty());
                assert(xadd.accepts(nrun));

                for(size_t i=tgt.rows;i>0;--i)
                {
                    Writable<T>       &tgt_i = tgt[i];
                    const Readable<U> &lhs_i = lhs[i];
                    for(size_t j=ncol;j>0;--j)
                    {
                        for(size_t k=nrun;k>0;--k)
                        {
                            const W p = Transmogrify<W>::Product(lhs_i[k], rhs[k][j]);
                            xadd << p;
                        }
                        tgt_i[j] = xadd.sum();
                    }
                }
            }

            //__________________________________________________________________
            //
            //
            //
            //! Matrix Multiplication, Right Transposed
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MMul(Matrix<T>          &tgt,
                      const Matrix<U>    &lhs,
                      const TransposeOf_ &,
                      const Matrix<V>    &rhs,
                      MultiAdd<W>        &xma)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);
                
                const size_t ncol = tgt.cols;
                const size_t nrun = lhs.cols;
                XAdd<W>     &xadd = xma.make(nrun);
                for(size_t i=tgt.rows;i>0;--i)
                {
                    Writable<T>       &tgt_i = tgt[i];
                    const Readable<U> &lhs_i = lhs[i];
                    for(size_t j=ncol;j>0;--j)
                    {
                        tgt_i[j] = DotProduct<W>::Of_(lhs_i,rhs[j],xadd);
                    }

                }
            }

        }

    }

}

#endif
