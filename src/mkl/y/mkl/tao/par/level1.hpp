//! \file

#ifndef Y_MKL_ParTao1_Included
#define Y_MKL_ParTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/engine.hpp"

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

                //! load on range
                template <typename TARGET, typename SOURCE> inline
                void Load(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    if(range.length<=0) return;
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] = source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source, Engine &engine)
            {
                assert(target.size()<=source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Load<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! save on range
                template <typename TARGET, typename SOURCE> inline
                void Save(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    if(range.length<=0) return;
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] = source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source, Engine &engine)
            {
                assert(target.size()>=source.size());
                engine.setup(source.size());
                engine.in1D(Parallel::Save<TARGET,SOURCE>,target,source);
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
                //! add on range
                template <typename TARGET, typename SOURCE> inline
                void Add(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    if(range.length<=0) return;
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] += source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add( TARGET &target, SOURCE &source, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Add<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! add on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Add(Engine1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    if(range.length<=0) return;
                    for(size_t i=range.latest;i>=range.offset;--i)
                        target[i] += factor * source[i];
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void Add( TARGET &target, T factor, SOURCE &source, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Add<TARGET,T,SOURCE>,target,factor,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! add on range
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Add(Engine1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    size_t     i=range.latest;
                    for(size_t j=range.length;j>0;--j,--i)
                        target[i] = source[i] + factor * vector[i];
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void Add(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Add<TARGET,SOURCE,T,VECTOR>,target,source,factor,vector);
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
                //! sub on range
                template <typename TARGET, typename SOURCE> inline
                void Sub(Engine1D &range, TARGET &target, SOURCE &source)
                {
                    size_t     i=range.latest;
                    for(size_t j=range.length;j>0;--j,--i)
                        target[i] -= source[i];
                }

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Sub( TARGET &target, SOURCE &source, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Sub<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! sub on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Sub(Engine1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    size_t     i=range.latest;
                    for(size_t j=range.length;j>0;--j,--i)
                        target[i] -= factor * source[i];
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void Sub( TARGET &target, T factor, SOURCE &source, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Sub<TARGET,T,SOURCE>,target,factor,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //! sub on range
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Sub(Engine1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    size_t     i=range.latest;
                    for(size_t j=range.length;j>0;--j,--i)
                        target[i] = source[i] - factor * vector[i];
                }
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void Sub(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Engine &engine)
            {
                assert(target.size()==source.size());
                engine.setup(target.size());
                engine.in1D(Parallel::Sub<TARGET,SOURCE,T,VECTOR>,target,source,factor,vector);
            }

        }

    }

}

#endif

