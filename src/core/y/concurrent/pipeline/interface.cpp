
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
            static const TaskUUID j1 = 1;
            Coerce(tid) = Max<TaskUUID>(j1,j1+tid);
        }

        TaskUUID Pipeline:: load(const Task &task)
        {
            assert(tid>0);
            try {
                suspend(); // no-throw
                const TaskUUID ans = enqueue(task,tid);
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

