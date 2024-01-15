//! \file

#ifndef Y_Concurrent_Task_Callback_Included
#define Y_Concurrent_Task_Callback_Included 1

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
        //! Storing functionoids
        //
        //
        //______________________________________________________________________
        template <typename FUNCTION, typename TLIST>
        class Callback : public Runnable, private Binder<TLIST>
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
            inline explicit Callback(const FUNCTION &fn) :
            Runnable(), Binder<TLIST>(), func(fn)
            {
            }
            
            //! setup1
            inline explicit Callback(const FUNCTION &fn, Param1 p1) :
            Runnable(), Binder<TLIST>(p1), func(fn)
            {
            }
            
            //! setup2
            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2) :
            Runnable(), Binder<TLIST>(p1,p2), func(fn)
            {
            }
            
            //! setup3
            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3) :
            Runnable(), Binder<TLIST>(p1,p2,p3), func(fn)
            {
            }
            
            //! setup4
            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3, Param4 p4) :
            Runnable(), Binder<TLIST>(p1,p2,p3,p4), func(fn)
            {
            }
            
            
            //! cleanup
            inline virtual ~Callback() noexcept {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback);
            FUNCTION func;
            
            inline virtual void run(const ThreadContext &ctx)
            {
                static const ArgsType args = {};
                call(ctx,args);
            }
            
            inline void call(const ThreadContext &ctx, const Int2Type<0> &) { func(ctx);                     }
            inline void call(const ThreadContext &ctx, const Int2Type<1> &) { func(ctx,arg1);                }
            inline void call(const ThreadContext &ctx, const Int2Type<2> &) { func(ctx,arg1,arg2);           }
            inline void call(const ThreadContext &ctx, const Int2Type<3> &) { func(ctx,arg1,arg2,arg3);      }
            inline void call(const ThreadContext &ctx, const Int2Type<4> &) { func(ctx,arg1,arg2,arg3,arg4); }
            
        };
        
    }
}

#endif

