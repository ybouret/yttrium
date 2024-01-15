
//! \file


#ifndef Y_Concurrent_Pipeline_Dispatcher_Invoke_Included
#define Y_Concurrent_Pipeline_Dispatcher_Invoke_Included 1

#include "y/concurrent/pipeline/dispatcher.hpp"
#include "y/concurrent/pipeline/task.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate methods and arguments to invoke in Dispatcher
        //
        //
        //______________________________________________________________________
        template <typename ENGINE,typename TLIST>
        class Invoke : public Task, public DispatcherConnect
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_BINDER_ECHO(TLIST);                               //!< aliases
            typedef Compute<ENGINE>              ComputeEngine; //!< alias
            typedef Writable< ComputeEngine >    ComputeKernel; //!< alias


            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________

            //! engine.method(ctx)
            template <typename METHOD> static inline
            TaskUUID Call(Dispatcher<ENGINE> &kernel,
                          METHOD              method)
            {
                const Invoke proc(kernel,method);
                return Load(kernel,proc);
            }

            //! engine.method(ctx,param1)
            template <typename METHOD> static inline
            TaskUUID Call(Dispatcher<ENGINE> &kernel,
                          METHOD              method,
                          Param1              param1)
            {
                const Invoke proc(kernel,method,param1);
                return Load(kernel,proc);
            }

            //! engine.method(ctx,param1,param2)
            template <typename METHOD> static inline
            TaskUUID Call(Dispatcher<ENGINE> &kernel,
                          METHOD              method,
                          Param1              param1,
                          Param2              param2)
            {
                const Invoke proc(kernel,method,param1,param2);
                return Load(kernel,proc);
            }

            //! engine.method(ctx,param1,param2,param3)
            template <typename METHOD> static inline
            TaskUUID Call(Dispatcher<ENGINE> &kernel,
                          METHOD              method,
                          Param1              param1,
                          Param2              param2,
                          Param3              param3)
            {
                const Invoke proc(kernel,method,param1,param2,param3);
                return Load(kernel,proc);
            }

            //! engine.method(ctx,param1,param2,param3,param4)
            template <typename METHOD> static inline
            TaskUUID Call(Dispatcher<ENGINE> &kernel,
                          METHOD              method,
                          Param1              param1,
                          Param2              param2,
                          Param3              param3,
                          Param4              param4)
            {
                const Invoke proc(kernel,method,param1,param2,param3,param4);
                return Load(kernel,proc);
            }

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            virtual ~Invoke() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Invoke);
            template <typename METHOD>
            inline explicit Invoke(ComputeKernel &kernel,
                                   METHOD         method) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method) )
            {
            }

            template <typename METHOD>
            inline explicit Invoke(ComputeKernel &kernel,
                                   METHOD         method,
                                   Param1         param1) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method,param1) )
            {
            }

            template <typename METHOD>
            inline explicit Invoke(ComputeKernel &kernel,
                                   METHOD         method,
                                   Param1         param1,
                                   Param2         param2) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method,param1,param2) )
            {
            }


            template <typename METHOD>
            inline explicit Invoke(ComputeKernel &kernel,
                                   METHOD         method,
                                   Param1         param1,
                                   Param2         param2,
                                   Param3         param3) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method,param1,param2,param3) )
            {
            }

            template <typename METHOD>
            inline explicit Invoke(ComputeKernel &kernel,
                                   METHOD         method,
                                   Param1         param1,
                                   Param2         param2,
                                   Param3         param3,
                                   Param4         param4) :
            Task( new Mission<ENGINE,METHOD,TLIST>(kernel,method,param1,param2,param3,param4) )
            {
            }

        };
    }

}

#endif
