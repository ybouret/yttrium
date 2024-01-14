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

        //______________________________________________________________________
        //
        //
        //
        //! Base class for Pipeline
        //
        //
        //______________________________________________________________________
        class Tasks
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! push a new task
            virtual Task::ID push(const Task &) = 0;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Tasks() noexcept {}
        protected:
            explicit Tasks() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tasks);
        };


        //______________________________________________________________________
        //
        //
        //
        //! Mission for tasks
        //
        //
        //______________________________________________________________________
        template <typename TLIST>
        class Mission : public Task
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            Y_BINDER_ECHO(TLIST); //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! copy (increase reference)
            inline Mission(const Mission &other) noexcept : Task(other) {}

            //! cleanup
            inline virtual ~Mission() noexcept {}

            //__________________________________________________________________
            //
            //
            // Wrappers with no argument
            //
            //__________________________________________________________________

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

            //! call into task
            template <typename FUNCTION> static inline
            Task::ID Call(Tasks &tasks, const FUNCTION &fn)
            {
                const Mission mission(Functionoid,fn);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            Task::ID Invoke(Tasks &tasks,
                            OBJECT &o,
                            METHOD  m)
            {
                const Mission mission(CxxMethodOf,o,m);
                return tasks.push(mission);
            }


            //__________________________________________________________________
            //
            //
            // Wrappers with one argument
            //
            //__________________________________________________________________

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

            //! call into task
            template <typename FUNCTION> static inline
            Task::ID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1)
            {
                const Mission mission(Functionoid,fn,p1);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            Task::ID Invoke(Tasks &tasks,
                            OBJECT &o,
                            METHOD  m,
                            Param1  p1)
            {
                const Mission mission(CxxMethodOf,o,m,p1);
                return tasks.push(mission);
            }


            //__________________________________________________________________
            //
            //
            // Wrappers with two arguments
            //
            //__________________________________________________________________

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


            //! call into task
            template <typename FUNCTION> static inline
            Task::ID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1, Param2 p2)
            {
                const Mission mission(Functionoid,fn,p1,p2);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            Task::ID Invoke(Tasks &tasks,
                            OBJECT &o,
                            METHOD  m,
                            Param1  p1,
                            Param2  p2)
            {
                const Mission mission(CxxMethodOf,o,m,p1,p2);
                return tasks.push(mission);
            }


            //__________________________________________________________________
            //
            //
            // Wrappers with three arguments
            //
            //__________________________________________________________________

            //! create from fn(context,p1,p2,p3)
            template <typename FUNCTION>
            inline explicit Mission(const Functionoid_ &,
                                    const FUNCTION     &fn,
                                    Param1              p1,
                                    Param2              p2,
                                    Param3              p3) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1,p2,p3) )
            {

            }

            //! create from object.method(context,p1,p2,p3)
            template <typename OBJECT, typename METHOD> inline
            explicit Mission(const CxxMethodOf_ &,
                             OBJECT             &o,
                             METHOD              m,
                             Param1              p1,
                             Param2              p2,
                             Param3              p3) :
            Task( new Command<OBJECT,METHOD,TLIST>(o,m,p1,p2,p3) )
            {
            }

            //! call into task
            template <typename FUNCTION> static inline
            Task::ID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3)
            {
                const Mission mission(Functionoid,fn,p1,p2,p3);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            Task::ID Invoke(Tasks &tasks,
                            OBJECT &o,
                            METHOD  m,
                            Param1  p1,
                            Param2  p2,
                            Param3  p3)
            {
                const Mission mission(CxxMethodOf,o,m,p1,p2,p3);
                return tasks.push(mission);
            }


            //__________________________________________________________________
            //
            //
            // Wrappers with four arguments
            //
            //__________________________________________________________________

            //! create from fn(context,p1,p2,p3,p4)
            template <typename FUNCTION>
            inline explicit Mission(const Functionoid_ &,
                                    const FUNCTION     &fn,
                                    Param1              p1,
                                    Param2              p2,
                                    Param3              p3,
                                    Param4              p4) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1,p2,p3,p4) )
            {

            }

            //! create from object.method(context,p1,p2,p3,p4)
            template <typename OBJECT, typename METHOD> inline
            explicit Mission(const CxxMethodOf_ &,
                             OBJECT             &o,
                             METHOD              m,
                             Param1              p1,
                             Param2              p2,
                             Param3              p3,
                             Param4              p4) :
            Task( new Command<OBJECT,METHOD,TLIST>(o,m,p1,p2,p3,p4) )
            {
            }


        private:
            Y_DISABLE_ASSIGN(Mission);
        };

    }

}

#endif
