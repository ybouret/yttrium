//! \file

#ifndef Y_MKL_ParTao1_Included
#define Y_MKL_ParTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/par/driver.hpp"
#include "y/mkl/tao/ops.hpp"

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
                //--------------------------------------------------------------
                //! Load/Save kernel
                //--------------------------------------------------------------
                template <typename TARGET, typename SOURCE> struct LoadKernel
                {
                    //! apply
                    inline void operator()(const size_t i)
                    {
                        target[i] = source[i];
                    }

                    TARGET &target; //!< alias
                    SOURCE &source; //!< alias
                };

                //--------------------------------------------------------------
                //! load on range
                //--------------------------------------------------------------
                template <typename TARGET, typename SOURCE> inline
                void Load(Driver1D &range, TARGET &target, SOURCE &source)
                {
                    LoadKernel<TARGET,SOURCE> op = { target, source };
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

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()>=source.size());
                driver.setup(source.size());
                driver.in1D(Parallel::Load<TARGET,SOURCE>,target,source);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //--------------------------------------------------------------
                //! binary ops kernel
                //--------------------------------------------------------------
                template <typename TARGET, typename SOURCE, typename PROC>
                struct BinaryKernel
                {

                    //! apply
                    inline void operator()(const size_t i)
                    {
                        proc(target[i],source[i]);
                    }

                    TARGET &target; //!< alias
                    SOURCE &source; //!< alias
                    PROC   &proc;   //!< alias
                };

                //______________________________________________________________
                //
                //! call proc(target[i],source[i])
                //______________________________________________________________
                template <typename TARGET, typename SOURCE, typename PROC> inline
                void Binary(Driver1D &range, TARGET &target, SOURCE &source, PROC &proc)
                {
                    BinaryKernel<TARGET,SOURCE,PROC>  op = { target, source, proc };
                    range->sweep(op);
                };

            }

            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add( TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                typedef void    (*PROC)(TGT &, const SRC &);
                static PROC       proc = Ops<TGT,SRC>::Add;

                driver.setup(target.size());
                driver.in1D(Parallel::Binary<TARGET,SOURCE,PROC>,target,source,proc);
            }

            //__________________________________________________________________
            //
            //! target -= source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Sub( TARGET &target, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                typedef void    (*PROC)(TGT &, const SRC &);
                static PROC       proc = Ops<TGT,SRC>::Sub;

                driver.setup(target.size());
                driver.in1D(Parallel::Binary<TARGET,SOURCE,PROC>,target,source,proc);
            }

        }

        namespace Tao
        {
            namespace Parallel
            {
                //--------------------------------------------------------------
                //! ternary ops kernel
                //--------------------------------------------------------------
                template <typename TARGET, typename FACTOR, typename SOURCE, typename PROC>
                struct TernaryKernel
                {
                    //! apply
                    inline void operator()(const size_t i)
                    {
                        typedef typename TARGET::Type TGT;
                        const TGT rhs = Transmogrify<TGT>::Product(factor,source[i]);
                        proc(target[i],rhs);
                    }

                    TARGET       &target; //!< alias
                    const FACTOR &factor; //!< alias
                    SOURCE       &source; //!< alias
                    PROC         &proc;   //!< alias
                };

                //______________________________________________________________
                //
                //! call proc(target[i],factor*source[i])
                //______________________________________________________________
                template <typename TARGET, typename FACTOR, typename SOURCE, typename PROC> inline
                void Ternary(Driver1D &range, TARGET &target, const FACTOR &factor, SOURCE &source, PROC &proc)
                {
                    TernaryKernel<TARGET,FACTOR,SOURCE,PROC> op = { target, factor, source, proc };
                    range->sweep(op);
                };
            }

            //__________________________________________________________________
            //
            //! target = factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulSet( TARGET &target, T factor, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                typedef void    (*PROC)(TGT &, const SRC &);
                static PROC       proc = Ops<TGT,SRC>::Set;

                driver.setup(target.size());
                driver.in1D(Parallel::Ternary<TARGET,T,SOURCE,PROC>,target,factor,source,proc);
            }

            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulAdd( TARGET &target, T factor, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                typedef void    (*PROC)(TGT &, const SRC &);
                static PROC       proc = Ops<TGT,SRC>::Add;

                driver.setup(target.size());
                driver.in1D(Parallel::Ternary<TARGET,T,SOURCE,PROC>,target,factor,source,proc);
            }

            //__________________________________________________________________
            //
            //! target -= factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void MulSub( TARGET &target, T factor, SOURCE &source, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef typename TARGET::Type TGT;
                typedef typename SOURCE::Type SRC;
                typedef void    (*PROC)(TGT &, const SRC &);
                static PROC       proc = Ops<TGT,SRC>::Sub;

                driver.setup(target.size());
                driver.in1D(Parallel::Ternary<TARGET,T,SOURCE,PROC>,target,factor,source,proc);
            }


        }

        namespace Tao
        {
            namespace Parallel
            {
                //______________________________________________________________
                //
                //! kernel for quaternary ops
                //______________________________________________________________
                template <typename TARGET,  typename SOURCE, typename FACTOR, typename VECTOR, typename PROC>
                struct QuaternaryKernel
                {

                    //! apply
                    inline void operator()(const size_t i)
                    {
                        typedef typename TARGET::Type TGT;
                        typedef typename SOURCE::Type SRC;
                        const TGT rhs = Transmogrify<TGT>::Product(factor,vector[i]);
                        proc(target[i]=To<TGT,SRC>::Get(source[i]),rhs);
                    }

                    TARGET       &target; //!< alias
                    SOURCE       &source; //!< alias
                    const FACTOR &factor; //!< alias
                    VECTOR       &vector; //!< alias
                    PROC         &proc;   //!< alias
                };

                //______________________________________________________________
                //
                //! call target[i] = proc(source[i],factor*vector[i])
                //______________________________________________________________
                template <typename TARGET,  typename SOURCE, typename FACTOR, typename VECTOR, typename PROC> inline
                void Quaternary(Driver1D &range, TARGET &target,  SOURCE &source, const FACTOR &factor, VECTOR &vector, PROC &proc )
                {
                    QuaternaryKernel<TARGET,SOURCE,FACTOR,VECTOR,PROC> op = { target, source, factor, vector, proc };
                    range->sweep(op);
                };

            }



            //__________________________________________________________________
            //
            //! target = source + factor * vector
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void MulAdd(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef void (*PROC)(TGT &, const TGT &);
                static PROC    proc = Ops<TGT,TGT>::Add;
                
                driver.setup(target.size());
                driver.in1D(Parallel::Quaternary<TARGET,SOURCE,T,VECTOR,PROC>,target,source,factor,vector,proc);
            }


            //__________________________________________________________________
            //
            //! target = source - factor * vector
            //__________________________________________________________________
            template <typename TARGET,  typename SOURCE, typename T, typename VECTOR> inline
            void MulSub(TARGET &target,  SOURCE &source, const T &factor, VECTOR &vector, Driver &driver)
            {
                assert(target.size()==source.size());
                typedef typename TARGET::Type TGT;
                typedef void (*PROC)(TGT &, const TGT &);
                static PROC    proc = Ops<TGT,TGT>::Sub;

                driver.setup(target.size());
                driver.in1D(Parallel::Quaternary<TARGET,SOURCE,T,VECTOR,PROC>,target,source,factor,vector,proc);
            }

        }

    }

}

#endif

