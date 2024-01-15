
//! \file

#ifndef Y_Concurrent_Job_Included
#define Y_Concurrent_Job_Included 1

#include "y/concurrent/pipeline/task/manager.hpp"
#include "y/concurrent/pipeline/task.hpp"


namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! a Job is a concrete Task
        //
        //
        //______________________________________________________________________
        template <typename TLIST>
        class Job : public Task
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
            inline Job(const Job &other) noexcept : Task(other) {}

            //! cleanup
            inline virtual ~Job() noexcept {}

            //__________________________________________________________________
            //
            //
            // Wrappers with no argument
            //
            //__________________________________________________________________

            //! create from functioniod : fn(context)
            template <typename FUNCTION> inline
            explicit Job(const Functionoid_       &,
                         const FUNCTION     &fn) :
            Task(  new Callback<FUNCTION,NullType>(fn) )
            {
            }

            //! create from host.meth(context)
            template <typename OBJECT, typename METHOD> inline
            explicit Job(const CxxMethodOf_ &,
                         OBJECT             &host,
                         METHOD              meth) :
            Task( new Command<OBJECT,METHOD,NullType>(host,meth) )
            {
            }

            //! call into task
            template <typename FUNCTION> static inline
            TaskUUID Launch(TaskManager    &tasks,
                            const FUNCTION &fn)
            {
                const Job job(Functionoid,fn);
                return tasks.load(job);
            }

            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskUUID Invoke(TaskManager &tasks,
                            OBJECT      &host,
                            METHOD       meth)
            {
                const Job job(CxxMethodOf,host,meth);
                return tasks.load(job);
            }

            //__________________________________________________________________
            //
            //
            // Wrappers with one argument
            //
            //__________________________________________________________________

            //! create from fn(context,p1)
            template <typename FUNCTION>
            inline explicit Job(const Functionoid_ &,
                                const FUNCTION     &fn,
                                Param1              p1) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1) )
            {

            }


            //! create from host.meth(context,p1)
            template <typename OBJECT, typename METHOD> inline
            explicit Job(const CxxMethodOf_ &,
                         OBJECT             &host,
                         METHOD              meth,
                         Param1              p1) :
            Task( new Command<OBJECT,METHOD,TLIST>(host,meth,p1) )
            {
            }


            //! call into task
            template <typename FUNCTION> static inline
            TaskUUID Launch(TaskManager    &tasks,
                            const FUNCTION &fn,
                            Param1          p1)
            {
                const Job job(Functionoid,fn,p1);
                return tasks.load(job);
            }


            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskUUID Invoke(TaskManager &tasks,
                            OBJECT      &host,
                            METHOD       meth,
                            Param1       p1)
            {
                const Job job(CxxMethodOf,host,meth,p1);
                return tasks.load(job);
            }

            //__________________________________________________________________
            //
            //
            // Wrappers with two arguments
            //
            //__________________________________________________________________

            //! create from fn(context,p1,p2)
            template <typename FUNCTION>
            inline explicit Job(const Functionoid_ &,
                                const FUNCTION     &fn,
                                Param1              p1,
                                Param2              p2) :
            Task( new Callback<FUNCTION,TLIST>(fn,p1,p2) )
            {

            }


            //! create from host.meth(context,p1)
            template <typename OBJECT, typename METHOD> inline
            explicit Job(const CxxMethodOf_ &,
                         OBJECT             &host,
                         METHOD              meth,
                         Param1              p1,
                         Param2              p2) :
            Task( new Command<OBJECT,METHOD,TLIST>(host,meth,p1,p2) )
            {
            }


            //! call into task
            template <typename FUNCTION> static inline
            TaskUUID Launch(TaskManager    &tasks,
                            const FUNCTION &fn,
                            Param1          p1,
                            Param2          p2)
            {
                const Job job(Functionoid,fn,p1,p2);
                return tasks.load(job);
            }


            //! invoke into task
            template <typename OBJECT, typename METHOD> static inline
            TaskUUID Invoke(TaskManager &tasks,
                            OBJECT      &host,
                            METHOD       meth,
                            Param1       p1,
                            Param2       p2)
            {
                const Job job(CxxMethodOf,host,meth,p1,p2);
                return tasks.load(job);
            }

        private:
            Y_DISABLE_ASSIGN(Job);
        };
    }

}

#endif

