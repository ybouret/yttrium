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

                
                //! load on range
                template <typename TARGET, typename SOURCE> inline
                void Load(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    struct Op 
                    {
                        TARGET &target;
                        SOURCE &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] = source[i];
                        }
                    };
                    Op op = { target, source };
                    range->sweep(op);
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
                    struct Op
                    {
                        TARGET &target;
                        SOURCE &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] = source[i];
                        }
                    };
                    Op op = { target, source };
                    range->sweep(op);
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

                //! add on range
                template <typename TARGET, typename SOURCE> inline
                void Add(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    struct Op
                    {
                        TARGET &target;
                        SOURCE &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] += source[i];
                        }
                    };
                    Op op = { target, source };
                    range->sweep(op);
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

                //! add on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Add(Driver1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    typedef typename TARGET::Type TGT;
                    struct Op
                    {
                        TARGET  &target;
                        const T &factor;
                        SOURCE  &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] += Transmogrify<TGT>::Product(factor,source[i]);
                        }
                    };
                    Op op = { target, factor, source };
                    range->sweep(op);
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
                //! mul add on ranges
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Add(Driver1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    typedef typename TARGET::Type TGT;
                    typedef typename SOURCE::Type SRC;
                    struct Op
                    {
                        TARGET  &target;
                        SOURCE  &source;
                        const T &factor;
                        VECTOR  &vector;

                        inline void operator()(const size_t i)
                        {
                            target[i] = To<TGT,SRC>::Get(source[i]) + Transmogrify<TGT>::Product(factor,vector[i]);
                        }
                    };
                    Op op = { target, source, factor, vector };
                    range->sweep(op);
                }
            }

            //__________________________________________________________________
            //
            //! target = source + factor * vector
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

                //! sub on range
                template <typename TARGET, typename SOURCE> inline
                void Sub(Driver1D &range, TARGET &target, SOURCE &source)
                {               
                    struct Op
                    {
                        TARGET &target;
                        SOURCE &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] -= source[i];
                        }
                    };
                    Op op = { target, source };
                    range->sweep(op);
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

                //! sub on range
                template <typename TARGET, typename T, typename SOURCE> inline
                void Sub(Driver1D &range, TARGET &target, const T &factor, SOURCE &source)
                {
                    typedef typename TARGET::Type TGT;
                    struct Op
                    {
                        TARGET  &target;
                        const T &factor;
                        SOURCE  &source;
                        inline void operator()(const size_t i)
                        {
                            target[i] -= Transmogrify<TGT>::Product(factor,source[i]);
                        }
                    };
                    Op op = { target, factor, source };
                    range->sweep(op);
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

                //! sub on range
                template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
                void Sub(Driver1D &range, TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector)
                {
                    typedef typename TARGET::Type TGT;
                    typedef typename SOURCE::Type SRC;
                    struct Op
                    {
                        TARGET  &target;
                        SOURCE  &source;
                        const T &factor;
                        VECTOR  &vector;

                        inline void operator()(const size_t i)
                        {
                            target[i] = To<TGT,SRC>::Get(source[i]) - Transmogrify<TGT>::Product(factor,vector[i]);
                        }
                    };
                    Op op = { target, source, factor, vector };
                    range->sweep(op);
                }
            }

            //__________________________________________________________________
            //
            //! target = source - factor * vector
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

