
#include "y/concurrent/pipeline/task.hpp"
#include "y/system/error.hpp"
#include <cerrno>
#include <iostream>

namespace Yttrium
{

    namespace Concurrent
    {

        Task:: Task(Runnable *userCode) noexcept : code( userCode ) {
            if(0==code) Libc::CriticalError(EINVAL, "NULL user code in Concurent::Task");
            initialize();
        }

        void Task:: initialize() noexcept
        {
            assert(0!=code);
            code->withhold();
        }

        Task:: ~Task() noexcept
        {
            assert(0!=code);
            if(code->liberate())
                delete code;
            code=0;
        }

        Task:: Task(const Task &task) noexcept : code( task.code )
        {
            initialize();
        }


        void Task:: process(const ThreadContext &ctx)
        {
            assert(0!=code);
            code->run(ctx);
        }

    }
}

#include "y/concurrent/pipeline/task/manager.hpp"


namespace Yttrium
{

    namespace Concurrent
    {

        TaskUUID TaskManager:: run(Runnable *runnable)
        {
            const Task task(runnable);
            return load(task);
        }
    }

}

