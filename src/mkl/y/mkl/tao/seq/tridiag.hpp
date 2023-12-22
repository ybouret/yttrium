

//! \file

#ifndef Y_MKL_SeqTaoTridiag_Included
#define Y_MKL_SeqTaoTridiag_Included 1

#include "y/mkl/api.hpp"
#include "y/mkl/tao/ops.hpp"
#include "y/mkl/algebra/tridiag.hpp"
#include "y/mkl/tao/transmogrify.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {

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
                void TriDiagMul(TARGET &            target,
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

                    {
                        const U s = Trans::Product(M.b[1],source[1]) + Trans::Product(M.c[1],source[2]);
                        proc(target[1],s);
                    }
                    const size_t nm=n-1;
                    for(size_t i=nm,im=i-1,ip=i+1;i>1;--i,--im,--ip)
                    {
                        const U A = Trans::Product(M.a[i],source[im]);
                        const U B = Trans::Product(M.b[i],source[i]);
                        const U C = Trans::Product(M.c[i],source[ip]);
                        const U s = Antelope::Sum3<U>::Of(A,B,C);
                        proc(target[i],s);
                    }
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
            void TriDiagMul(TARGET &            target,
                            const TriDiag<T>   &M,
                            SOURCE             &source,
                            const Type2Type<U>  innerType)
            {
                Cog::TriDiagMul(target,M,source,Ops<typename TARGET::Type,U>::Set,innerType);
            }

            //__________________________________________________________________
            //
            //
            //! target += M*source
            //
            //__________________________________________________________________
            template <
            typename TARGET,
            typename T,
            typename SOURCE,
            typename U> inline
            void TriDiagMulAdd(TARGET &            target,
                               const TriDiag<T>   &M,
                               SOURCE             &source,
                               const Type2Type<U>  inner)
            {
                Cog::TriDiagMul(target,M,source,Ops<typename TARGET::Type,U>::Add,inner);
            }

            //__________________________________________________________________
            //
            //
            //! target -= M*source
            //
            //__________________________________________________________________
            template <
            typename TARGET,
            typename T,
            typename SOURCE,
            typename U> inline
            void TriDiagMulSub(TARGET &            target,
                               const TriDiag<T>   &M,
                               SOURCE             &source,
                               const Type2Type<U>  inner)
            {
                Cog::TriDiagMul(target,M,source,Ops<typename TARGET::Type,U>::Sub,inner);
            }
        }

    }

}

#endif

