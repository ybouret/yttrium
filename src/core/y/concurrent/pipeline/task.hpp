//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/runnable.hpp"
#include "y/type/binder.hpp"
#include "y/type/ints.hpp"
#include "y/config/shallow.hpp"


namespace Yttrium
{
    Y_SHALLOW_DECL(Functionoid); //!< alias
    Y_SHALLOW_DECL(CxxMethodOf); //!< alias

    namespace Concurrent
    {


        template <typename FUNCTION, typename TLIST>
        class Callback : public Runnable, private Binder<TLIST>
        {
        public:
            Y_BINDER_ARGS(TLIST);

            inline explicit Callback(const FUNCTION &fn) :
            Runnable(), Binder<TLIST>(), func(fn)
            {
            }

            inline explicit Callback(const FUNCTION &fn, Param1 p1) :
            Runnable(), Binder<TLIST>(p1), func(fn)
            {
            }

            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2) :
            Runnable(), Binder<TLIST>(p1,p2), func(fn)
            {
            }

            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3) :
            Runnable(), Binder<TLIST>(p1,p2,p3), func(fn)
            {
            }

            inline explicit Callback(const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3, Param4 p4) :
            Runnable(), Binder<TLIST>(p1,p2,p3,p4), func(fn)
            {
            }


            inline virtual ~Callback() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Callback);
            FUNCTION func;
            
            inline virtual void run(const ThreadContext &ctx)
            {
                static const ArgsType args = {};
                call(ctx,args);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<0> &)
            {
                func(ctx);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<1> &)
            {
                func(ctx,arg1);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<2> &)
            {
                func(ctx,arg1,arg2);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<3> &)
            {
                func(ctx,arg1,arg2,arg3);
            }

            inline void call(const ThreadContext &ctx, const Int2Type<4> &)
            {
                func(ctx,arg1,arg2,arg3,arg4);
            }
        };

        //______________________________________________________________________
        //
        //
        //! encapsulate call to OBJECT.METHOD call
        //
        //______________________________________________________________________
        template <typename OBJECT, typename METHOD, typename TLIST>
        class Command : public Runnable, private Binder<TLIST>
        {
        public:
            Y_BINDER_ARGS(TLIST);

            inline explicit Command(OBJECT &o, METHOD m) noexcept :
            Runnable(),
            Binder<TLIST>(),
            host(o),
            meth(m) {}


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



        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate all different (ref-counted) Runnable for Pipeline
        //
        //
        //______________________________________________________________________
        class Task
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef UnsignedInt<sizeof(void*)>::Type ID; //!< identifier
           
            //! status of the task
            enum Status
            {
                Success, //!< done
                Pending, //!< pending
                Running  //!< running
            };

            //__________________________________________________________________
            //
            //
            // C++ : allowing to make pre-compiled, re-usable tasks
            //
            //__________________________________________________________________
            Task(Runnable   *) noexcept; //!< setup from user's runnable code
            Task(const Task &) noexcept; //!< shared copy
            ~Task()            noexcept; //!< cleanup


            //! create from function/functionoid (full copy)
            template <typename FUNCTION> inline
            Task(const FUNCTION     &fn) :
            code(new Callback<FUNCTION,NullType>(fn))
            { initialize(); }

            //! create from object+method
            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT             &o,
                 METHOD              m) :
            code( new Command<OBJECT,METHOD,NullType>(o,m) )
            { initialize(); }

#if 0
            template <typename FUNCTION, typename TLIST> inline
            Task(const Functionoid_            &,
                 const FUNCTION                &fn,
                 typename Binder<TLIST>::Param1 p1) :
            code( new Callback<FUNCTION,TLIST>(fn,p1) )
            {
            }
#endif

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

            
        private:
            Y_DISABLE_ASSIGN(Task);
            void initialize() noexcept; //!< withold code
            Runnable *code;
        };

    }

}

#endif
