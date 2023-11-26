
#include "y/concurrent/pipeline/interface.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Pipeline:: ~Pipeline() noexcept
        {
        }

        Pipeline:: Pipeline() noexcept : tid(1)
        {
        }

        void Pipeline:: upgrade() noexcept
        {
            static const Task::ID j1 = 1;
            Coerce(tid) = Max<Task::ID>(j1,j1+tid);
        }

        Task::ID Pipeline:: push(const Task &task)
        {
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


    }

}

