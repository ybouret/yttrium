
//! \file

#ifndef Y_Threads_Included
#define Y_Threads_Included 1


#include "y/concurrent/topology.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/thread/context.hpp"
#include "y/concurrent/wire.hpp"

#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Threads;

        class Agent : public ThreadContext, public Wire
        {
        public:
            explicit Agent(const size_t sz, const size_t rk, Lockable &mx, Threads &);
            virtual ~Agent() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Agent);
        };

        typedef Memory::Wad<Agent,Memory::Dyadic> Agency;

        class Threads : public Agency
        {
        public:
            explicit Threads(const Topology &);
            virtual ~Threads() noexcept;

            Mutex     access;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Threads);
            friend class Agent;

            Condition     waitCV;
            Condition     doneCV;
            size_t        size;
            Agent * const crew;
            size_t        nrun;

            void          loop(Agent &agent) noexcept;
            static void   Launch(Threads &threads, Agent &agent) noexcept;
            void          quit() noexcept;
        };

    }

}

#endif
