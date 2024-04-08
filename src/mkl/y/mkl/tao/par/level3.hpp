//! \file

#ifndef Y_MKL_ParTao3_Included
#define Y_MKL_ParTao3_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/par/driver.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/tao/ops.hpp"

#if 1
namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {

            namespace Parallel
            {

                //! proc(T &, const W &)
                template <typename T, typename U, typename V, typename W, typename PROC> inline
                void MatMulOp(Driver2D        &range,
                              Matrix<T>       &tgt,
                              const Matrix<U> &lhs,
                              const Matrix<V> &rhs,
                              PROC            &proc)
                {
                    const Strip *strip = range.strip;
                    if(!strip) return;
                    assert(0!=range.tile);
                    assert(range.tile->size>0);

                    const size_t      nrun = lhs.cols;
                    Antelope::Add<W> &xadd = range.xadd<W>();
                    assert(xadd.isEmpty());
                    assert(xadd.accepts(nrun));

                    for(size_t s=range.tile->size;s>0;--s)
                    {
                        const Strip       &here  = *(++strip);
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
                            const W  res = xadd.sum();
                            proc(tgt_i[j],res);
                        }
                    }
                }

            }

            //__________________________________________________________________
            //
            //! tgt = proc(lhs,rhs)
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W, typename PROC>  inline
            void MatMulCall(Matrix<T>          &tgt,
                            const Matrix<U>    &lhs,
                            const Matrix<V>    &rhs,
                            Antelope::Caddy<W> &xma,
                            PROC               &proc,
                            Driver             &driver)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.cols);
                assert(lhs.cols==rhs.rows);

                driver.setup(tgt);                                         // parallel tiles of target
                driver.in2D.link(xma.make(driver.in2D.size(),lhs.cols)); // one xadd per tile

                volatile Driver::Unlink2D willUnlink(driver.in2D);
                driver.in2D(Parallel::MatMulOp<T,U,V,W,PROC>,tgt,lhs,rhs,proc);
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

                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Set;
                MatMulCall(tgt,lhs,rhs,xma,proc,driver);
            }

            //__________________________________________________________________
            //
            //
            //
            //! parallel matrix multiplication/addition
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MatMulAdd(Matrix<T>          &tgt,
                           const Matrix<U>    &lhs,
                           const Matrix<V>    &rhs,
                           Antelope::Caddy<W> &xma,
                           Driver             &driver)
            {

                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Add;
                MatMulCall(tgt,lhs,rhs,xma,proc,driver);
            }

            //__________________________________________________________________
            //
            //
            //
            //! parallel matrix multiplication/subtraction
            //
            //
            //__________________________________________________________________
            template <typename T, typename U, typename V, typename W>  inline
            void MatMulSub(Matrix<T>          &tgt,
                           const Matrix<U>    &lhs,
                           const Matrix<V>    &rhs,
                           Antelope::Caddy<W> &xma,
                           Driver             &driver)
            {
                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Sub;
                MatMulCall(tgt,lhs,rhs,xma,proc,driver);
            }




            namespace Parallel
            {

                //______________________________________________________________
                //
                //
                //! compute tgt[range] = lhs * rhs'
                //
                //______________________________________________________________
                template <typename T, typename U, typename V, typename W, typename PROC> inline
                void MatMulRightTransposeOp(Driver2D        &range,
                                            Matrix<T>       &tgt,
                                            const Matrix<U> &lhs,
                                            const Matrix<V> &rhs,
                                            PROC            &proc)
                {
                    const Strip *strip = range.strip;
                    if(!strip) return;
                    assert(0!=range.tile);
                    assert(range.tile->size>0);

                    Antelope::Add<W> &xadd = range.xadd<W>();
                    assert(xadd.isEmpty());
                    assert(xadd.accepts(lhs.cols));

                    for(size_t s=range.tile->size;s>0;--s)
                    {
                        const Strip       &here  = *(++strip);
                        const size_t       i     = here.irow;
                        Writable<T>       &tgt_i = tgt[i];
                        const Readable<U> &lhs_i = lhs[i];
                        for(size_t j=here.icol,nc=here.ncol;nc>0;--nc,++j)
                        {
                            assert(xadd.isEmpty());
                            assert(xadd.accepts(lhs.cols));
                            const W res  = DotProduct<W>::Of_(lhs_i,rhs[j],xadd);
                            proc(tgt_i[j],res);
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
            template <typename T, typename U, typename V, typename W, typename PROC>  inline
            void MatMulCall(Matrix<T>          &tgt,
                            const Matrix<U>    &lhs,
                            const TransposeOf_ &,
                            const Matrix<V>    &rhs,
                            Antelope::Caddy<W> &xma,
                            PROC               &proc,
                            Driver             &driver)
            {
                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);

                driver.setup(tgt);                                                           // parallel tiles of target
                driver.in2D.link(xma.make(driver.in2D.size(),lhs.cols));                     // one xadd per tile
                volatile Driver::Unlink2D willUnlink(driver.in2D);                           // cleanup anyhow
                driver.in2D(Parallel::MatMulRightTransposeOp<T,U,V,W,PROC>,tgt,lhs,rhs,proc);  // call
            }

            //! tgt = lhs * rhs'
            template <typename T, typename U, typename V, typename W>  inline
            void MatMul(Matrix<T>          &tgt,
                        const Matrix<U>    &lhs,
                        const TransposeOf_ &_,
                        const Matrix<V>    &rhs,
                        Antelope::Caddy<W> &xma,
                        Driver             &driver)
            {
                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Set;

                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);
                MatMulCall(tgt,lhs,_,rhs,xma,proc,driver);
            }

            //! tgt += lhs * rhs'
            template <typename T, typename U, typename V, typename W>  inline
            void MatMulAdd(Matrix<T>          &tgt,
                           const Matrix<U>    &lhs,
                           const TransposeOf_ &_,
                           const Matrix<V>    &rhs,
                           Antelope::Caddy<W> &xma,
                           Driver             &driver)
            {
                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Add;

                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);
                MatMulCall(tgt,lhs,_,rhs,xma,proc,driver);
            }

            //! tgt -= lhs * rhs'
            template <typename T, typename U, typename V, typename W>  inline
            void MatMulSub(Matrix<T>          &tgt,
                           const Matrix<U>    &lhs,
                           const TransposeOf_ &_,
                           const Matrix<V>    &rhs,
                           Antelope::Caddy<W> &xma,
                           Driver             &driver)
            {
                typedef void (*PROC)(T &, const W &);
                static PROC proc = Ops<T,W>::Sub;

                assert(tgt.rows==lhs.rows);
                assert(tgt.cols==rhs.rows);
                assert(lhs.cols==rhs.cols);
                MatMulCall(tgt,lhs,_,rhs,xma,proc,driver);
            }



            namespace Parallel
            {

                template <typename T, typename ARRAY, typename V, typename PROC>
                struct DiagMatMulInfo
                {
                    Matrix<T>       &tgt;
                    ARRAY           &lhs;
                    const Matrix<V> &rhs;
                    PROC            &proc;

                    inline void operator()(const size_t i)
                    { 
                        typedef typename ARRAY::Type U;
                        typedef To<T,U>              U2T;
                        typename U2T::ReturnType lambda = U2T::Get(lhs[i]);
                        Writable<T>            & tgt_i  = tgt[i];
                        const Readable<V>      & rhs_i  = rhs[i];
                        const size_t              ncol  = tgt.cols;

                        for(size_t j=ncol;j>0;--j)
                        {
                            const T res = lambda * To<T,V>::Get(rhs_i[j]);
                            proc(tgt_i[j],res);
                        }
                    }
                };

                //! fill rows of range with proc
                template <typename T, typename ARRAY, typename V, typename PROC>
                void DiagMatMulOp(Driver1D        &range,
                                  Matrix<T>       &tgt,
                                  ARRAY           &lhs,
                                  const Matrix<V> &rhs,
                                  PROC            &proc)
                {
                    assert( tgt.rows == lhs.size() );
                    assert( tgt.cols == rhs.cols   );

                    DiagMatMulInfo<T,ARRAY,V,PROC> op = { tgt, lhs, rhs, proc };
                    range->sweep(op);

                }

                //! compute proc(tgt,lhs*rhs)*
                template <typename T, typename ARRAY, typename V, typename PROC> inline
                void DiagMatMulCall(Driver         &driver,
                                    Matrix<T>       &tgt,
                                    ARRAY           &lhs,
                                    const Matrix<V> &rhs,
                                    PROC            &proc)
                {
                    assert( tgt.rows == lhs.size() );
                    assert( tgt.cols == rhs.cols   );

                    driver.setup(tgt.rows);
                    driver.in1D(DiagMatMulOp<T,ARRAY,V,PROC>,tgt,lhs,rhs,proc);
                }
            }




            //__________________________________________________________________
            //
            //
            //
            //! Set Diagonal Matrix times Matrix
            //
            //
            //__________________________________________________________________
            template <typename T, typename ARRAY, typename V>
            void DiagMatMul(Matrix<T>       &tgt,
                            ARRAY           &lhs,
                            const Matrix<V> &rhs,
                            Driver          &driver)
            {
                typedef void (*PROC)(T &, const T &);
                static PROC    proc = Ops<T,T>::Set;
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                Parallel::DiagMatMulCall<T,ARRAY,V,PROC>(driver,tgt,lhs,rhs,proc);
            }

            //__________________________________________________________________
            //
            //
            //
            //! Add Diagonal Matrix times Matrix
            //
            //
            //__________________________________________________________________
            template <typename T, typename ARRAY, typename V>
            void DiagMatMulAdd(Matrix<T>       &tgt,
                               ARRAY           &lhs,
                               const Matrix<V> &rhs,
                               Driver          &driver)
            {
                typedef void (*PROC)(T &, const T &);
                static PROC    proc = Ops<T,T>::Add;
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                Parallel::DiagMatMulCall<T,ARRAY,V,PROC>(driver,tgt,lhs,rhs,proc);
            }

            //__________________________________________________________________
            //
            //
            //
            //! Sub Diagonal Matrix times Matrix
            //
            //
            //__________________________________________________________________
            template <typename T, typename ARRAY, typename V>
            void DiagMatMulSub(Matrix<T>       &tgt,
                               ARRAY           &lhs,
                               const Matrix<V> &rhs,
                               Driver          &driver)
            {
                typedef void (*PROC)(T &, const T &);
                static PROC    proc = Ops<T,T>::Sub;
                assert( tgt.rows == lhs.size() );
                assert( tgt.cols == rhs.cols   );
                Parallel::DiagMatMulCall<T,ARRAY,V,PROC>(driver,tgt,lhs,rhs,proc);
            }
        }

    }

}
#endif

#endif
