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
                //______________________________________________________________
                //
                //
                //! compute tgt[range] = lhs * rhs
                //
                //______________________________________________________________
                template <typename T, typename U, typename V, typename W> inline
                void MatMul(Engine2D        &range,
                            Matrix<T>       &tgt,
                            const Matrix<U> &lhs,
                            const Matrix<V> &rhs)
                {

                    if( range.isEmpty() ) return;

                    const size_t      nrun = lhs.cols;
                    Antelope::Add<W> &xadd = range.xadd<W>();
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
                                const W p = Transmogrify<W>::Product(lhs_i[k], rhs[k][j]);
                                xadd << p;
                            }
                            tgt_i[j] = xadd.sum();
                        }
                    }
                }
            }



            //__________________________________________________________________
            //
            //
            //
            //! parallel matrix multiplication
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MatMul(Matrix<T>          &tgt,
                        const Matrix<U>    &lhs,
                        const Matrix<V>    &rhs,
                        Antelope::Caddy<W> &xma,
                        Engine             &engine)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);

                engine.setup(tgt);                                         // parallel tiles of target
                engine.in2D.attach(xma.make(engine.in2D.size(),lhs.cols)); // one xadd per tile

                volatile Engine::Clean2D detach(engine.in2D);
                engine.in2D(Parallel::MatMul<T,U,V,W>,tgt,lhs,rhs);
            }


            namespace Parallel
            {
                //______________________________________________________________
                //
                //
                //! compute tgt[range] = lhs * rhs
                //
                //______________________________________________________________
                template <typename T, typename U, typename V, typename W> inline
                void MatMulRightTranspose(Engine2D        &range,
                                          Matrix<T>       &tgt,
                                          const Matrix<U> &lhs,
                                          const Matrix<V> &rhs)
                {

                    if( range.isEmpty() ) return;

                    Antelope::Add<W> &xadd = range.xadd<W>();
                    assert(xadd.isEmpty());
                    assert(xadd.accepts(lhs.cols));

                    for(size_t s=range->size;s>0;--s)
                    {
                        const Strip       &here  = range(s);
                        const size_t       i     = here.irow;
                        Writable<T>       &tgt_i = tgt[i];
                        const Readable<U> &lhs_i = lhs[i];
                        for(size_t j=here.icol,nc=here.ncol;nc>0;--nc,++j)
                        {
                            assert(xadd.isEmpty());
                            assert(xadd.accepts(lhs.cols));
                            tgt_i[j] = DotProduct<W>::Of_(lhs_i,rhs[j],xadd);
                        }
                    }
                }
            }


            //__________________________________________________________________
            //
            //
            //
            //! Parallel Matrix Multiplication, right transpose
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MatMul(Matrix<T>          &tgt,
                        const Matrix<U>    &lhs,
                        const TransposeOf_ &,
                        const Matrix<V>    &rhs,
                        Antelope::Caddy<W> &xma,
                        Engine             &engine)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);

                engine.setup(tgt);                                                 // parallel tiles of target
                engine.in2D.attach(xma.make(engine.in2D.size(),lhs.cols));         // one xadd per tile
                volatile Engine::Clean2D detach(engine.in2D);                      // cleanup anyhow
                engine.in2D(Parallel::MatMulRightTranspose<T,U,V,W>,tgt,lhs,rhs);  // call
            }

            namespace Parallel
            {
                //! fill rows of range
                template <typename T, typename ARRAY, typename V>
                void DiagMatMul(Engine1D        &range,
                                Matrix<T>       &tgt,
                                ARRAY           &lhs,
                                const Matrix<V> &rhs)
                {
                    assert( tgt.rows == lhs.size() );
                    assert( tgt.cols == rhs.cols   );

                    typedef typename ARRAY::Type U;
                    typedef To<T,U>              U2T;

                    if(range.length<=0) return;
                    const size_t ncol = tgt.cols;
                    for(size_t i=range.latest;i>=range.offset;--i)
                    {
                        typename U2T::ReturnType lambda = U2T::Get(lhs[i]);
                        Writable<T>            & tgt_i  = tgt[i];
                        const Readable<V>      & rhs_i  = rhs[i];
                        for(size_t j=ncol;j>0;--j)
                            tgt_i[j] = lambda * To<T,V>::Get(rhs_i[j]);
                    }
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
            template <typename T, typename ARRAY, typename V>
            void DiagMatMul(Matrix<T>       &tgt,
                            ARRAY           &lhs,
                            const Matrix<V> &rhs,
                            Engine          &engine)
            {
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                
                engine.setup(tgt.rows);
                engine.in1D(Parallel::DiagMatMul<T,ARRAY,V>,tgt,lhs,rhs);
            }
        }

    }

}

#endif
