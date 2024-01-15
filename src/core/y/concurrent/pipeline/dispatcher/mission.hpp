//! \file


#ifndef Y_Concurrent_Pipeline_Dispatcher_Mission_Included
#define Y_Concurrent_Pipeline_Dispatcher_Mission_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/type/binder.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate methods and arguments to invoke
        //
        //
        //______________________________________________________________________
        template <typename ENGINE, typename METHOD, typename TLIST>
        class Mission : public Runnable, public Binder<TLIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_BINDER_ECHO(TLIST);                              //!< aliases
            Y_BINDER_ARGS(TLIST);                              //!< aliases
            typedef Compute<ENGINE>           ComputeEngine;   //!< alias
            typedef Writable< ComputeEngine > ComputeKernel;   //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Mission() noexcept {}

            //! setup with no arguments : method(ctx)
            inline explicit Mission(ComputeKernel &kernel,
                                    METHOD         method) noexcept :
            Runnable(),
            Binder<TLIST>(),
            core(kernel),
            meth(method)
            {
            }


            //! setup with one argument: method(ctx,param1)
            inline explicit Mission(ComputeKernel &kernel,
                                    METHOD         method,
                                    Param1         param1) noexcept :
            Runnable(),
            Binder<TLIST>(param1),
            core(kernel),
            meth(method)
            {
            }

            //! setup with two arguments: method(ctx,param1,param2)
            inline explicit Mission(ComputeKernel &kernel,
                                    METHOD         method,
                                    Param1         param1,
                                    Param2         param2) noexcept :
            Runnable(),
            Binder<TLIST>(param1,param2),
            core(kernel),
            meth(method)
            {
            }

            //! setup with three arguments: method(ctx,param1,param2,param3)
            inline explicit Mission(ComputeKernel &kernel,
                                    METHOD         method,
                                    Param1         param1,
                                    Param2         param2,
                                    Param3         param3) noexcept :
            Runnable(),
            Binder<TLIST>(param1,param2,param3),
            core(kernel),
            meth(method)
            {
            }

            //! setup with four arguments: method(ctx,param1,param2,param3,param4)
            inline explicit Mission(ComputeKernel &kernel,
                                    METHOD         method,
                                    Param1         param1,
                                    Param2         param2,
                                    Param3         param3,
                                    Param4         param4) noexcept :
            Runnable(),
            Binder<TLIST>(param1,param2,param3,param4),
            core(kernel),
            meth(method)
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mission);
            ComputeKernel  &core;
            METHOD          meth;

            //! select and call engine.method(ctx,...)
            inline virtual void run(const ThreadContext &ctx)
            {
                static const ArgsType args = {};             // select call
                ComputeEngine        &host = core[ctx.indx]; // select host, deriving from context
                call(host,args);
            }

            inline void call(ComputeEngine &host, const Int2Type<0> &) { (host.*meth)(host);                     }
            inline void call(ComputeEngine &host, const Int2Type<1> &) { (host.*meth)(host,arg1);                }
            inline void call(ComputeEngine &host, const Int2Type<2> &) { (host.*meth)(host,arg1,arg2);           }
            inline void call(ComputeEngine &host, const Int2Type<3> &) { (host.*meth)(host,arg1,arg2,arg3);      }
            inline void call(ComputeEngine &host, const Int2Type<4> &) { (host.*meth)(host,arg1,arg2,arg3,arg4); }


        };

    }

}


#endif
