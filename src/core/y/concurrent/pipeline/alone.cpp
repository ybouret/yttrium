#include "y/concurrent/pipeline/alone.hpp"
#include "y/concurrent/pipeline/task.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Alone:: ~Alone() noexcept
        {
        }

        Alone:: Alone(const char *id) noexcept :
        mine(id),
        self(mine)
        {
        }

        TaskFlag Alone:: query(const TaskUUID)    const noexcept
        {
            return TaskSuccess;
        }

        const char * const Alone::CallSign = "Concurrent::Alone";

        const char * Alone:: callSign() const noexcept { return CallSign; }

        size_t Alone:: size() const noexcept
        {
            return 1;
        }

        Alone::ConstType & Alone:: operator[](const size_t
#ifndef NDEBUG
                                              indx
#endif
        ) const noexcept
        {
            assert(1==indx);
            return self;
        }


        void Alone:: suspend() noexcept
        {
            mine.lock();
        }

        void Alone:: restart() noexcept
        {
            mine.unlock();
        }


        size_t   Alone:: cargo() const noexcept
        {
            return 0;
        }

        TaskUUID Alone:: enqueue(const Task &task, const TaskUUID tid)
        {
            Coerce(task).process(self);
            return tid;
        }

        void Alone:: flush() noexcept
        {

        }


        Lockable & Alone:: sync() noexcept { return mine; }

    }

}

