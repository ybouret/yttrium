
//! \file

#ifndef Y_Threads_Included
#define Y_Threads_Included 1


#include "y/concurrent/topology.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/thread/context.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Concurrent
    {


        class Agent : public ThreadContext
        {
        public:

            explicit Agent(const size_t sz, const size_t rk, Lockable &mx);
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
            Condition waitCV;
            size_t    agents;
            Agent * const agent;
        };

    }

}

#endif
