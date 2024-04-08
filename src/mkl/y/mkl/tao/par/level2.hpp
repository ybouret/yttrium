//! \file

#ifndef Y_MKL_ParTao2_Included
#define Y_MKL_ParTao2_Included 1


#include "y/mkl/tao/seq/level1.hpp"
#include "y/mkl/tao/par/driver.hpp"
#include "y/container/matrix.hpp"

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
                //! multiplication kernel
                //______________________________________________________________
                template <typename TARGET, typename T, typename SOURCE, typename U, typename PROC>
                struct MulKernel
                {
                    //! apply
                    inline void operator()(const size_t row)
                    {
                        assert(xadd.isEmpty());
                        const U result = DotProduct<U>::Of_(M[row],source,xadd);
                        proc(target[row],result);
                    }
                    
                    TARGET           &target; //!< alias
                    const Matrix<T>  &M;      //!< alias
                    SOURCE           &source; //!< alias
                    PROC             &proc;   //!< alias
                    Antelope::Add<U> &xadd;   //!< alias
                };

                //! Mul on range
                template <typename TARGET, typename T, typename SOURCE, typename U, typename PROC> inline
                void Mul(Driver1D &range, TARGET &target, const Matrix<T> &M, SOURCE &source, PROC &proc)
                {
                    MulKernel<TARGET,T,SOURCE,U,PROC> op = { target, M, source, proc, range.xadd<U>() };
                    range->sweep(op);
                }
            }

            //__________________________________________________________________
            //
            //! parallel multiplication Set/Add/Sub
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE, typename U, typename PROC> inline
            void MulOp(TARGET &            target,
                       const Matrix<T>    &M,
                       SOURCE             &source,
                       Antelope::Caddy<U> &xma,
                       Driver             &driver,
                       PROC               &proc)
            {

                assert( target.size() == M.rows );
                assert( source.size() == M.cols );

                const size_t   para = driver.in1D.size();
                driver.setup(M.rows);                    // process rows in parallel
                driver.in1D.link(xma.make(para,M.cols)); // with help

                volatile Driver::Unlink1D willClean(driver.in1D);
                driver.in1D(Parallel::Mul<TARGET,T,SOURCE,U,PROC>,target,M,source,proc);
            }

            //__________________________________________________________________
            //
            //! target = M*source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void Mul(TARGET &            target,
                     const Matrix<T>    &M,
                     SOURCE             &source,
                     Antelope::Caddy<U> &xma,
                     Driver             &driver)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC      proc = Ops<TGT,U>::Set;
                MulOp(target,M,source,xma,driver,proc);
            }

            //__________________________________________________________________
            //
            //! target += M*source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void MulAdd(TARGET &            target,
                        const Matrix<T>    &M,
                        SOURCE             &source,
                        Antelope::Caddy<U> &xma,
                        Driver             &driver)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC   proc = Ops<TGT,U>::Add;
                MulOp(target,M,source,xma,driver,proc);
            }

            //__________________________________________________________________
            //
            //! target += M*source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE, typename U> inline
            void MulSub(TARGET &            target,
                        const Matrix<T>    &M,
                        SOURCE             &source,
                        Antelope::Caddy<U> &xma,
                        Driver             &driver)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC   proc = Ops<TGT,U>::Sub;
                MulOp(target,M,source,xma,driver,proc);
            }

        }

    }

}

#endif
