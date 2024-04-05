//! \file

#ifndef Y_MKL_ParTao3_Included
#define Y_MKL_ParTao3_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/par/driver.hpp"
#include "y/container/matrix.hpp"

#if 1
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
                void MatMul(Driver2D        &range,
                            Matrix<T>       &tgt,
                            const Matrix<U> &lhs,
                            const Matrix<V> &rhs)
                {

#if 0
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
#endif
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
                        Driver             &driver)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);

                driver.setup(tgt);                                         // parallel tiles of target
                driver.in2D.link(xma.make(driver.in2D.size(),lhs.cols)); // one xadd per tile

                volatile Driver::Unlink2D willUnlink(driver.in2D);
                driver.in2D(Parallel::MatMul<T,U,V,W>,tgt,lhs,rhs);
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
                void MatMulRightTranspose(Driver2D        &range,
                                          Matrix<T>       &tgt,
                                          const Matrix<U> &lhs,
                                          const Matrix<V> &rhs)
                {
#if 0
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
#endif
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
                        Driver             &driver)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);

                driver.setup(tgt);                                                 // parallel tiles of target
                driver.in2D.link(xma.make(driver.in2D.size(),lhs.cols));         // one xadd per tile
                volatile Driver::Unlink2D willUnlink(driver.in2D);                      // cleanup anyhow
                driver.in2D(Parallel::MatMulRightTranspose<T,U,V,W>,tgt,lhs,rhs);  // call
            }

            namespace Parallel
            {
                //! fill rows of range
                template <typename T, typename ARRAY, typename V>
                void DiagMatMul(Driver1D        &range,
                                Matrix<T>       &tgt,
                                ARRAY           &lhs,
                                const Matrix<V> &rhs)
                {
                    assert( tgt.rows == lhs.size() );
                    assert( tgt.cols == rhs.cols   );

                    typedef typename ARRAY::Type U;
                    typedef To<T,U>              U2T;

#if 0
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
#endif
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
                            Driver          &driver)
            {
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                
                driver.setup(tgt.rows);
                driver.in1D(Parallel::DiagMatMul<T,ARRAY,V>,tgt,lhs,rhs);
            }
        }

    }

}
#endif

#endif
