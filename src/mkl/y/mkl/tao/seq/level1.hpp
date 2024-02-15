//! \file


#ifndef Y_MKL_SeqTao1_Included
#define Y_MKL_SeqTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/multiadd.hpp"
#include "y/mkl/antelope/caddy.hpp"
#include "y/mkl/tao/transmogrify.hpp"

namespace Yttrium
{
    namespace MKL
    {



        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source)
            {
                assert(target.size()<=source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] = source[i];
            }

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source)
            {
                assert(target.size()>=source.size());
                for(size_t i=source.size();i>0;--i)
                    target[i] = source[i];
            }

        }


        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add(TARGET &target, SOURCE &source)
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += source[i];
            }


#if 1
            //__________________________________________________________________
            //
            //! target = lhs+rhs
            //__________________________________________________________________
            template <typename TARGET, typename LHS, typename RHS>   inline
            void Add(TARGET &target, LHS &lhs, RHS &rhs)
            {
                assert(target.size()==lhs.size());
                assert(target.size()==rhs.size());
                typedef typename TARGET::Type TTYPE;
                typedef typename LHS::Type    LTYPE;
                typedef typename RHS::Type    RTYPE;
                for(size_t i=target.size();i>0;--i)
                    target[i] = To<TTYPE,LTYPE>::Get(lhs[i]) + To<TTYPE,RTYPE>::Get(rhs[i]);
            }
#endif


            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulAdd( TARGET &target, T factor, SOURCE &source )
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                for(size_t i=target.size();i>0;--i)
                    target[i] += Transmogrify<TGT>::Product(factor,source[i]);
            }

            //__________________________________________________________________
            //
            //! target = source + factor * vector
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE, typename T, typename VECTOR>   inline
            void MulAdd( TARGET &target, SOURCE &source, T factor, VECTOR &vector)
            {
                assert(target.size()==source.size());
                assert(target.size()==vector.size());
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                for(size_t i=target.size();i>0;--i)
                    target[i] = To<TGT,SRC>::Get(source[i]) + Transmogrify<TGT>::Product(factor,vector[i]);
            }

        }

        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target -= source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Sub(TARGET &target, SOURCE &source)
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] -= source[i];
            }


            //__________________________________________________________________
            //
            //! target -= factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulSub(TARGET &target, T factor, SOURCE &source)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                for(size_t i=target.size();i>0;--i)
                    target[i] -= Transmogrify<TGT>::Product(factor,source[i]);
            }

            //__________________________________________________________________
            //
            //! target = source - factor * vector
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE, typename T, typename VECTOR>   inline
            void MulSub(TARGET &target, SOURCE &source, T factor, VECTOR &vector)
            {
                assert(target.size()==source.size());
                assert(target.size()==vector.size());
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                for(size_t i=target.size();i>0;--i)
                    target[i] = To<TGT,SRC>::Get(source[i]) - Transmogrify<TGT>::Product(factor,vector[i]);
            }

        }


        namespace Tao
        {
            //__________________________________________________________________
            //
            //
            //! dot product resulting in T
            //
            //__________________________________________________________________
            template <typename T>
            struct DotProduct
            {

                //______________________________________________________________
                //
                //! compute with prepared xadd
                //______________________________________________________________
                template <typename LHS, typename RHS> static inline
                T Of_(LHS &lhs, RHS &rhs, Antelope::Add<T> &xadd)
                {
                    const size_t size = lhs.size();
                    assert(lhs.size()==rhs.size());
                    assert(xadd.isEmpty());
                    assert(xadd.accepts(size));

                    for(size_t i=size;i>0;--i) {
                        const T prod = Transmogrify<T>::Product(lhs[i],rhs[i]);
                        xadd << prod;
                    }
                    return xadd.sum();
                }

                //______________________________________________________________
                //
                //! compute with any xadd
                //______________________________________________________________
                template <typename LHS, typename RHS> static inline
                T Of(LHS &lhs, RHS &rhs, MultiAdd<T>  &xma)
                {
                    assert(lhs.size()==rhs.size());
                    return Of_(lhs,rhs,xma.make(lhs.size()));
                }

            };

        }

        namespace Tao
        {
            namespace Cog
            {
                //__________________________________________________________________
                //
                //
                //! Squared Norm resulting in Scalar Type of T
                //
                //__________________________________________________________________
                template <typename T>
                struct SquaredNorm
                {
                    typedef T                              Type;        //!< alias
                    typedef typename ScalarFor<Type>::Type ScalarType;  //!< alias

                    //! compute |lhs|^2
                    template <typename LHS> static inline
                    ScalarType Of(LHS &lhs, MultiAdd<ScalarType> &xm)
                    {
                        typedef typename LHS::Type          U;
                        typedef typename ScalarFor<U>::Type ScalarU;

                        const size_t      n    = lhs.size();
                        XAdd<ScalarType> &xadd = xm.make(n);
                        for(size_t i=n;i>0;--i)
                        {
                            const ScalarU     l2 = MKL::Mod2<U>::Of( lhs[i] );
                            const ScalarType  x2 = Tao::To<ScalarType,ScalarU>::Get(l2);
                            xadd << x2;
                        }
                        return xadd.sum();
                    }

                    //! compute |primary-replica|^2, difference in PRIMARY::Type
                    template <typename PRIMARY, typename REPLICA> static inline
                    ScalarType Of(PRIMARY &primary, REPLICA &replica, MultiAdd<ScalarType> &xm)
                    {
                        typedef typename PRIMARY::Type                PrimaryType;
                        typedef typename ScalarFor<PrimaryType>::Type ScalarPrimary;

                        assert(primary.size()==replica.size());
                        const size_t      n    = primary.size();
                        XAdd<ScalarType> &xadd = xm.make(n);
                        for(size_t i=n;i>0;--i)
                        {
                            const PrimaryType   dd = primary[i] - Tao::To<PrimaryType,typename REPLICA::Type>::Get(replica[i]);
                            const ScalarPrimary d2 = MKL::Mod2<PrimaryType>::Of(dd);
                            const ScalarType    x2 = Tao::To<ScalarType,ScalarPrimary>::Get(d2);
                            xadd << x2;
                        }
                        return xadd.sum();
                    }

                };
            }

            //__________________________________________________________________
            //
            //
            //! Mod2 computation
            //
            //__________________________________________________________________
            template <typename T>
            class ComputeMod2
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Cog::SquaredNorm<T>        SqNrm;      //!< alias
                typedef typename SqNrm::ScalarType ScalarType; //!< aalias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit ComputeMod2() noexcept : sma() {} //!< setup
                inline virtual ~ComputeMod2() noexcept {}         //!< cleanup

                //! |primary|^2
                template <typename PRIMARY> inline
                ScalarType operator()(PRIMARY &primary)
                {
                    return SqNrm::Of(primary,sma);
                }

                //! |primary-replica|^2
                template <typename PRIMARY, typename REPLICA> inline
                ScalarType operator()(PRIMARY &primary, REPLICA &replica)
                {
                    return SqNrm::Of(primary,replica,sma);
                }

                //______________________________________________________________
                //
                //
                // members
                //
                //______________________________________________________________
                MultiAdd<ScalarType> sma; //!< scalar multi-add
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ComputeMod2);
            };


        }


    }

}

#endif

