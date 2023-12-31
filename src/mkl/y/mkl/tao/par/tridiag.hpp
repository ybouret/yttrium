//! \file

#ifndef Y_MKL_ParTaoTridiag_Included
#define Y_MKL_ParTaoTridiag_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/ops.hpp"
#include "y/mkl/algebra/tridiag.hpp"
#include "y/mkl/tao/transmogrify.hpp"
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
                //! compute target on defined range
                //
                //______________________________________________________________
                template <
                typename TARGET,
                typename T,
                typename SOURCE,
                typename U,
                typename PROC> inline
                void TriDiagMul(Engine1D           &range,
                                TARGET             &target,
                                const TriDiag<T>   &M,
                                SOURCE             &source,
                                PROC               &proc)
                {
                    typedef struct Transmogrify<U> Trans;

                    if(range.length<=0) return;
                    for(size_t i=range.latest,im=i,ip=i+1;i>=range.offset;--i)
                    {
                        --im;
                        assert(i-1==im);
                        assert(i+1==ip);
                        const U A = Trans::Product(M.a[i],source[im]);
                        const U B = Trans::Product(M.b[i],source[i]);
                        const U C = Trans::Product(M.c[i],source[ip]);
                        const U s = Antelope::Sum3<U>::Of(A,B,C);
                        proc(target[i],s);
                        --ip;
                    }

                }
            }

            namespace Cog
            {

                //______________________________________________________________
                //
                //
                //! proc(target,M*source)
                //
                //______________________________________________________________
                template <
                typename TARGET,
                typename T,
                typename SOURCE,
                typename U,
                typename PROC> inline
                void TriDiagMulParallel(Engine             &engine,
                                        TARGET             &target,
                                        const TriDiag<T>   &M,
                                        SOURCE             &source,
                                        PROC               &proc,
                                        const Type2Type<U> &)
                {
                    assert( target.size() == M.size );
                    assert( source.size() == M.size );
                    typedef struct Transmogrify<U> Trans;

                    const size_t n = M.size;
                    switch(n)
                    {
                        case 0: return;
                        case 1: target[1] = Trans::Product(M.b[1],source[1]); return;
                        default:
                            break;
                    }

                    //----------------------------------------------------------
                    // head
                    //----------------------------------------------------------
                    {
                        const U s = Trans::Product(M.b[1],source[1]) + Trans::Product(M.c[1],source[2]);
                        proc(target[1],s);
                    }

                    //----------------------------------------------------------
                    // bulk 2:n-1
                    //----------------------------------------------------------
                    const size_t nm=n-1;
                    if(n>2)
                    {
                        engine.in1D.init(2,nm,1);
                        engine.in1D(Tao::Parallel::TriDiagMul<TARGET,T,SOURCE,U,PROC>,target,M,source,proc);
                    }

                    //----------------------------------------------------------
                    // tail
                    //----------------------------------------------------------
                    {
                        const U s = Trans::Product(M.a[n],source[nm]) + Trans::Product(M.b[n],source[n]);
                        proc(target[n],s);
                    }
                }
            }


            //__________________________________________________________________
            //
            //
            //! target = M*source
            //
            //__________________________________________________________________
            template <
            typename TARGET,
            typename T,
            typename SOURCE,
            typename U> inline
            void TriDiagMul(Engine             &engine,
                            TARGET             &target,
                            const TriDiag<T>   &M,
                            SOURCE             &source,
                            const Type2Type<U>  innerType)
            {
                Cog::TriDiagMulParallel(engine,target,M,source,Ops<typename TARGET::Type,U>::Set,innerType);
            }

        }

    }


}

#endif

