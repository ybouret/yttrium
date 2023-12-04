
#include "y/concurrent/pipeline/interface.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Pipeline:: ~Pipeline() noexcept
        {
        }

        Pipeline:: Pipeline() noexcept :
        ThreadContexts(),
        tid(1)
        {
        }

        void Pipeline:: upgrade() noexcept
        {
            static const Task::ID j1 = 1;
            Coerce(tid) = Max<Task::ID>(j1,j1+tid);
        }

        Task::ID Pipeline:: push(const Task &task)
        {
            assert(tid>0);
            try {
                suspend(); // no-throw
                const Task::ID ans = enqueue(task,tid);
                restart(); // no-throw
                upgrade(); // no-throw
                return ans;
            }
            catch(...)
            {
                restart();
                throw;
            }

        }

        void Pipeline:: reset() noexcept
        {
            suspend();
            Coerce(tid) = 1;
            restart();
        }


    }

}

