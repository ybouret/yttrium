//! \file

#ifndef Y_MKL_ParTao1_Included
#define Y_MKL_ParTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/par/driver.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //--------------------------------------------------------------------//
        //                                                                    //
        //                                                                    //
        //                                                                    //
        //                        Load/Save                                   //
        //                                                                    //
        //                                                                    //
        //                                                                    //
        //--------------------------------------------------------------------//
        namespace Tao
        {


            namespace Parallel
            {

#define Y_MKL_TAO_IN1D(CODE)                                             \
const Mapping1D * const loop = range.loop;                               \
if(!loop) return;                                                        \
assert(Concurrent::ForLoopIncrease==loop->family);                       \
assert(loop->offset>0);                                                  \
const size_t offset = loop->offset;                                      \
for(size_t INDX=loop->latest;INDX>=offset;--INDX) Y_MKL_TAO_##CODE(INDX)


#define Y_MKL_TAO_LOAD(I) target[I] = source[I]

                //! load on range
                template <typename TARGET, typename SOURCE> inline
                void Load(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    Y_MKL_TAO_IN1D(LOAD);
                }

            }

            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()<=source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Load<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! save on range
                template <typename TARGET, typename SOURCE> inline
                void Save(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    Y_MKL_TAO_IN1D(LOAD);
                }

            }

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()>=source.size());
                driver.setup(source.size());
                driver.in1D(Parallel::Save<TARGET,SOURCE>,target,source);
            }

        }

        //--------------------------------------------------------------------//
        //                                                                    //
        //                                                                    //
        //                                                                    //
        // Add                                                                //
        //                                                                    //
        //                                                                    //
        //                                                                    //
        //--------------------------------------------------------------------//
        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_ADD(I) target[I] += source[I]

                //! add on range
                template <typename TARGET, typename SOURCE> inline
                void Add(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    Y_MKL_TAO_IN1D(ADD);
                }

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add( TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Add<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_MULADD(I) target[I] += Transmogrify<TGT>::Product(factor,source[I]);

                //! add on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Add(Driver1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    typedef typename TARGET::Type TGT;
                    Y_MKL_TAO_IN1D(MULADD);

                    //if(range.length<=0) return;
                    //for(size_t i=range.latest;i>=range.offset;--i)
                    //    target[i] += Transmogrify<TGT>::Product(factor,source[i]);
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulAdd( TARGET &target, T factor, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Add<TARGET,T,SOURCE>,target,factor,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_MULADDV(I) target[I] = To<TGT,SRC>::Get(source[I]) + Transmogrify<TGT>::Product(factor,vector[I]);
                //! mul add on ranges
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Add(Driver1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    typedef typename TARGET::Type TGT;
                    typedef typename SOURCE::Type SRC;
                    Y_MKL_TAO_IN1D(MULADDV);
                    //if(range.length<=0) return;
                    //for(size_t i=range.latest;i>=range.offset;--i)
                    //    target[i] = To<TGT,SRC>::Get(source[i]) + Transmogrify<TGT>::Product(factor,vector[i]);
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void MulAdd(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Add<TARGET,SOURCE,T,VECTOR>,target,source,factor,vector);
            }

        }


        //--------------------------------------------------------------------//
        //                                                                    //
        //                                                                    //
        //                                                                    //
        // Sub                                                                //
        //                                                                    //
        //                                                                    //
        //                                                                    //
        //--------------------------------------------------------------------//
        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_SUB(I) target[I] -= source[I]

                //! sub on range
                template <typename TARGET, typename SOURCE> inline
                void Sub(Driver1D &range, TARGET &target, SOURCE &source)
                {                
                    Y_MKL_TAO_IN1D(SUB);

                    //size_t     i=range.latest;
                    //for(size_t j=range.length;j>0;--j,--i)
                    //    target[i] -= source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Sub( TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Sub<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_MULSUB(I) target[I] -= Transmogrify<TGT>::Product(factor,source[I])

                //! sub on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Sub(Driver1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    typedef typename TARGET::Type TGT;
                    Y_MKL_TAO_IN1D(MULSUB);

                    //if(range.length<=0) return;
                    //for(size_t i=range.latest;i>=range.offset;--i)
                    //    target[i] -= Transmogrify<TGT>::Product(factor,source[i]);
                }
            }

            //__________________________________________________________________
            //
            //! target -= factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulSub( TARGET &target, T factor, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Sub<TARGET,T,SOURCE>,target,factor,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
#define Y_MKL_TAO_MULSUBV(I) target[I] = To<TGT,SRC>::Get(source[I]) - Transmogrify<TGT>::Product(factor,vector[I])
             
                //! sub on range
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Sub(Driver1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    typedef typename TARGET::Type TGT;
                    typedef typename SOURCE::Type SRC;
                    Y_MKL_TAO_IN1D(MULSUBV);

                    //if(range.length<=0) return;
                    //for(size_t i=range.latest;i>=range.offset;--i)
                    //    target[i] = To<TGT,SRC>::Get(source[i]) - Transmogrify<TGT>::Product(factor,vector[i]);
                }
            }

            //__________________________________________________________________
            //
            //! target = source - factor * source
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void MulSub(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Driver &driver)
            {
                assert(target.size()==source.size());
                driver.setup(target.size());
                driver.in1D(Parallel::Sub<TARGET,SOURCE,T,VECTOR>,target,source,factor,vector);
            }

        }

    }

}

#endif

