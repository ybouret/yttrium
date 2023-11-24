
//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/command.hpp"
#include "y/concurrent/pipeline/callback.hpp"


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
            // C++
            //
            //__________________________________________________________________
            Task(Runnable *)   noexcept; //!< setup from user's runnable code
            Task(const Task &) noexcept; //!< copy, shared copy
            ~Task()            noexcept; //!< cleanup

            //! create from function/functionoid (full copy)
            template <typename FUNCTION> inline
            Task(const FUNCTION &fn) : code( new Callback<FUNCTION>(fn) )       { initialize(); }

            //! create from object+method
            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT &o, METHOD m) : code( new Command<OBJECT,METHOD>(o,m) ) { initialize(); }

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
