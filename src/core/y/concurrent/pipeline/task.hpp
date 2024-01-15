//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/task/callback.hpp"
#include "y/concurrent/pipeline/task/command.hpp"
#include "y/concurrent/pipeline/task/uuid.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

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

#if 0
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
            virtual TaskUUID push(const Task &) = 0;

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
            TaskID Call(Tasks &tasks, const FUNCTION &fn)
            {
                const Mission mission(Functionoid,fn);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskID Invoke(Tasks &tasks,
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
            TaskID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1)
            {
                const Mission mission(Functionoid,fn,p1);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskID Invoke(Tasks &tasks,
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
            TaskID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1, Param2 p2)
            {
                const Mission mission(Functionoid,fn,p1,p2);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskID Invoke(Tasks &tasks,
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
            TaskID Call(Tasks &tasks, const FUNCTION &fn, Param1 p1, Param2 p2, Param3 p3)
            {
                const Mission mission(Functionoid,fn,p1,p2,p3);
                return tasks.push(mission);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskID Invoke(Tasks &tasks,
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
#endif
    }

}

#endif
