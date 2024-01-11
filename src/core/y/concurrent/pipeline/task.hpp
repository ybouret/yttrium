//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/command.hpp"
#include "y/concurrent/pipeline/callback.hpp"
#include "y/type/ints.hpp"
#include "y/config/shallow.hpp"


namespace Yttrium
{
    
    namespace Concurrent
    {


        //______________________________________________________________________
        //
        //
        //
        //! Encapsulate all different Runnable for Pipeline
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
            // C++
            //
            //__________________________________________________________________
            Task(Runnable *)   noexcept; //!< setup from user's runnable code
            Task(const Task &) noexcept; //!< shared copy
            ~Task()            noexcept; //!< cleanup

            // no args

            //! create from function/functionoid (full copy)
            template <typename FUNCTION> inline
            Task( const FUNCTION     &fn) :
            code( new Callback<FUNCTION>(fn) )
            { initialize(); }

            //! create from object+method
            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT             &o,
                 METHOD              m) :
            code( new Command<OBJECT,METHOD>(o,m) )
            { initialize(); }

            template <typename FUNCTION, typename ARG1> static inline
            Task Call(const FUNCTION &fn, ARG1 &a1)
            {
                const Task task( static_cast<Runnable *>(new Callback1<FUNCTION,ARG1>(fn,a1)) );
                return task;
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void process(const ThreadContext &); //!< run code on given context

            
        private:
            Y_DISABLE_ASSIGN(Task);
            void initialize() noexcept;
            Runnable *code;
        };

    }

}

#endif
