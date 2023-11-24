
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
            static const TaskID j1 = 1;
            Coerce(tid) = Max<TaskID>(j1,j1+tid);
        }

        TaskID Pipeline:: push(const Task &task)
        {
            try {
                suspend(); // no-throw
                const TaskID ans = enqueue(task,tid);
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

