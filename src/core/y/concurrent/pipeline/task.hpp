
//! \file

#ifndef Y_Concurrent_Task_Included
#define Y_Concurrent_Task_Included 1

#include "y/concurrent/pipeline/command.hpp"
#include "y/concurrent/pipeline/callback.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class Task
        {
        public:
            Task(Runnable *)   noexcept;
            Task(const Task &) noexcept;
            ~Task()            noexcept;

            template <typename FUNCTION> inline
            Task(const FUNCTION &fn) : code( new Callback<FUNCTION>(fn) )       { initialize(); }

            template <typename OBJECT, typename METHOD> inline
            Task(OBJECT &o, METHOD m) : code( new Command<OBJECT,METHOD>(o,m) ) { initialize(); }

            void process(const ThreadContext &);


        private:
            Y_DISABLE_ASSIGN(Task);
            void initialize() noexcept;
            Runnable *code;
        };

    }

}

#endif
