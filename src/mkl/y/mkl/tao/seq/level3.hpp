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
            void MatMul(Matrix<T>          &tgt,
                        const Matrix<U>    &lhs,
                        const Matrix<V>    &rhs,
                        Antelope::Caddy<W> &xma)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);
                const size_t      ncol = tgt.cols;
                const size_t      nrun = lhs.cols;
                Antelope::Add<W> &xadd = xma.make(nrun);

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
            void MatMul(Matrix<T>          &tgt,
                        const Matrix<U>    &lhs,
                        const TransposeOf_ &,
                        const Matrix<V>    &rhs,
                        Antelope::Caddy<W> &xma)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);

                const size_t      ncol = tgt.cols;
                const size_t      nrun = lhs.cols;
                Antelope::Add<W> &xadd = xma.make(nrun);
                for(size_t i=tgt.rows;i>0;--i)
                {
                    Writable<T>       &tgt_i = tgt[i];
                    const Readable<U> &lhs_i = lhs[i];
                    for(size_t j=ncol;j>0;--j)
                        tgt_i[j] = DotProduct<W>::Of_(lhs_i,rhs[j],xadd);
                }
            }

            //__________________________________________________________________
            //
            //
            //
            //! Diagonal Matrix times Matrix
            //
            //
            //__________________________________________________________________
            template <typename T, typename ARRAY, typename V> inline
            void DiagMatMul(Matrix<T> &tgt, ARRAY &lhs, const Matrix<V> &rhs)
            {
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                typedef typename ARRAY::Type U;
                typedef To<T,U>              U2T;

                const size_t ncol = tgt.cols;
                for(size_t i=tgt.rows;i>0;--i)
                {
                    typename U2T::ReturnType lambda = U2T::Get(lhs[i]);
                    Writable<T>            & tgt_i  = tgt[i];
                    const Readable<V>      & rhs_i  = rhs[i];
                    for(size_t j=ncol;j>0;--j)
                        tgt_i[j] = lambda * To<T,V>::Get(rhs_i[j]);
                }

            }

            //__________________________________________________________________
            //
            //
            //
            //! Gram Matrix of M
            //
            //
            //__________________________________________________________________
            template <typename T, typename U> inline
            void Gram(Matrix<T> &G, const Matrix<U> &M, Antelope::Caddy<T> &xma)
            {
                assert(G.rows==M.rows);
                assert(G.cols==G.rows);

                const size_t n = G.rows;
                for(size_t i=1;i<=n;++i)
                    for(size_t j=i;j<=n;++j)
                        G[i][j] = G[j][i] = DotProduct<T>::Of(M[i], M[j], xma);
                
            }

        }

    }

}

#endif
