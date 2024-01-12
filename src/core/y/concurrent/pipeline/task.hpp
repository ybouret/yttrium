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
            Y_BINDER_ECHO(TLIST);
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

            inline void call(const ThreadContext &ctx, const Int2Type<0> &) { func(ctx);                     }
            inline void call(const ThreadContext &ctx, const Int2Type<1> &) { func(ctx,arg1);                }
            inline void call(const ThreadContext &ctx, const Int2Type<2> &) { func(ctx,arg1,arg2);           }
            inline void call(const ThreadContext &ctx, const Int2Type<3> &) { func(ctx,arg1,arg2,arg3);      }
            inline void call(const ThreadContext &ctx, const Int2Type<4> &) { func(ctx,arg1,arg2,arg3,arg4); }
            
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
            Y_BINDER_ECHO(TLIST);
            Y_BINDER_ARGS(TLIST);

            inline explicit Command(OBJECT &o, METHOD m) noexcept :
            Runnable(), Binder<TLIST>(), host(o), meth(m) {}

            inline explicit Command(OBJECT &o, METHOD m, Param1 p1) noexcept :
            Runnable(), Binder<TLIST>(p1), host(o), meth(m) {}


            inline explicit Command(OBJECT &o, METHOD m, Param1 p1, Param2 p2) noexcept :
            Runnable(), Binder<TLIST>(p1,p2), host(o), meth(m) {}

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
            Task(const Task &) noexcept; //!< shared copy
            virtual ~Task()    noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

        protected:
            Task(Runnable   *) noexcept; //!< setup from user's runnable code
            void initialize()  noexcept; //!< withold code

        private:
            Y_DISABLE_ASSIGN(Task);
            Runnable *code;
        };

        template <typename TLIST>
        class Mission : public Task
        {
        public:
            Y_BINDER_ECHO(TLIST);

            inline Mission(const Mission &other) noexcept : Task(other) {}

            //! create from functioniod : fn(context)
            template <typename FUNCTION>
            inline explicit Mission(const Functionoid_ &,
                                    const FUNCTION     &fn) :
            Task(  new Callback<FUNCTION,NullType>(fn) )
            {
            }

            //! create from object.method(context)
            template <typename OBJECT, typename METHOD> inline
            explicit Mission(const CxxMethodOf_ &,
                             OBJECT             &o,
                             METHOD              m) :
            Task( new Command<OBJECT,METHOD,NullType>(o,m) )
            {
            }

            //! create from fn(context,p1)
            template <typename FUNCTION>
            inline explicit Mission(const Functionoid_ &,
                                    const FUNCTION     &fn,
                                    Param1              p1) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1) )
            {

            }

            //! create from object.method(context,p1)
            template <typename OBJECT, typename METHOD> inline
            explicit Mission(const CxxMethodOf_ &,
                             OBJECT             &o,
                             METHOD              m,
                             Param1              p1) :
            Task( new Command<OBJECT,METHOD,TLIST>(o,m,p1) )
            {
            }


            //! create from fn(context,p1,p2)
            template <typename FUNCTION>
            inline explicit Mission(const Functionoid_ &,
                                    const FUNCTION     &fn,
                                    Param1              p1,
                                    Param2              p2) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1,p2) )
            {

            }

            //! create from object.method(context,p1,p2)
            template <typename OBJECT, typename METHOD> inline
            explicit Mission(const CxxMethodOf_ &,
                             OBJECT             &o,
                             METHOD              m,
                             Param1              p1,
                             Param2              p2) :
            Task( new Command<OBJECT,METHOD,TLIST>(o,m,p1,p2) )
            {
            }


            inline virtual ~Mission() noexcept {}

        private:
            Y_DISABLE_ASSIGN(Mission);
        };

    }

}

#endif
