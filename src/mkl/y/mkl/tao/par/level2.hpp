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
                template <typename TARGET, typename T, typename SOURCE, typename U, typename PROC> inline
                void Mul(Engine1D &range, TARGET &target, const Matrix<T> &M, SOURCE &source, PROC &proc)
                {

                    if(range.length<=0) return;

                    Antelope::Add<U> &xadd = range.xadd<U>();
                    for(size_t row  = range.latest; row>=range.offset; --row)
                    {
                        assert(xadd.isEmpty());
                        const U result = DotProduct<U>::Of_(M[row],source,xadd);
                        proc(target[row],result);
                    }
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
                       Engine             &engine,
                       PROC               &proc)
            {

                assert( target.size() == M.rows );
                assert( source.size() == M.cols );

                const size_t   para = engine.in1D.size();
                engine.setup(M.rows);                      // process rows in parallel
                engine.in1D.attach(xma.make(para,M.cols)); // with help

                volatile Engine::Clean1D willClean(engine.in1D);
                engine.in1D(Parallel::Mul<TARGET,T,SOURCE,U,PROC>,target,M,source,proc);
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
                     Engine             &engine)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC      proc = Ops<TGT,U>::Set;

                MulOp(target,M,source,xma,engine,proc);
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
                        Engine             &engine)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC   proc = Ops<TGT,U>::Add;
                MulOp(target,M,source,xma,engine,proc);
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
                        Engine             &engine)
            {
                typedef typename TARGET::Type TGT;
                typedef void    (*PROC)(TGT &, const U &);
                static  PROC   proc = Ops<TGT,U>::Sub;
                MulOp(target,M,source,xma,engine,proc);
            }

        }

    }

}

#endif
