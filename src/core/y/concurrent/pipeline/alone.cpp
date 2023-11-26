

#include "y/concurrent/pipeline/alone.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        Alone:: ~Alone() noexcept
        {
        }

        Alone:: Alone(const char *id) noexcept :
        sync(id),
        self(sync) 
        {
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
            sync.lock();
        }

        void Alone:: restart() noexcept
        {
            sync.unlock();
        }


        Task::ID Alone:: enqueue(const Task &task, const Task::ID tid)
        {
            Coerce(task).process(self);
            return tid;
        }


    }

}

