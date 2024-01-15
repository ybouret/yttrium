
//! \file

#ifndef Y_Concurrent_Task_Command_Included
#define Y_Concurrent_Task_Command_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/concurrent/pipeline/task/defs.hpp"
#include "y/type/binder.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! encapsulate call to OBJECT.METHOD call
        //
        //
        //______________________________________________________________________
        template <typename OBJECT, typename METHOD, typename TLIST>
        class Command : public Runnable, private Binder<TLIST>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            Y_BINDER_ECHO(TLIST); //!< aliases
            Y_BINDER_ARGS(TLIST); //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup0
            inline explicit Command(OBJECT &o, METHOD m) noexcept :
            Runnable(), Binder<TLIST>(), host(o), meth(m) {}

            //! setup1
            inline explicit Command(OBJECT &o, METHOD m, Param1 p1) noexcept :
            Runnable(), Binder<TLIST>(p1), host(o), meth(m) {}

            //! setup2
            inline explicit Command(OBJECT &o, METHOD m, Param1 p1, Param2 p2) noexcept :
            Runnable(), Binder<TLIST>(p1,p2), host(o), meth(m) {}

            //! setup3
            inline explicit Command(OBJECT &o, METHOD m, Param1 p1, Param2 p2, Param3 p3) noexcept :
            Runnable(), Binder<TLIST>(p1,p2,p3), host(o), meth(m) {}

            //! setup3
            inline explicit Command(OBJECT &o, METHOD m, Param1 p1, Param2 p2, Param3 p3, Param4 p4) noexcept :
            Runnable(), Binder<TLIST>(p1,p2,p3,p4), host(o), meth(m) {}

            //! cleanup
            inline virtual ~Command() noexcept {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Command);
            inline virtual void run(const ThreadContext &ctx)
            {
                static const ArgsType args = {};
                call(ctx,args);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<0> &) { (host.*meth)(ctx);                }
            inline void call(const ThreadContext &ctx, const Int2Type<1> &) { (host.*meth)(ctx,arg1);           }
            inline void call(const ThreadContext &ctx, const Int2Type<2> &) { (host.*meth)(ctx,arg1,arg2);      }
            inline void call(const ThreadContext &ctx, const Int2Type<3> &) { (host.*meth)(ctx,arg1,arg2,arg3); }
            inline void call(const ThreadContext &ctx, const Int2Type<4> &) { (host.*meth)(ctx,arg1,arg2,arg4); }

            OBJECT &host;
            METHOD  meth;
        };



    }

}

#endif

