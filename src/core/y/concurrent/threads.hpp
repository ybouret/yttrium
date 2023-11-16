
//! \file

#ifndef Y_Threads_Included
#define Y_Threads_Included 1


#include "y/concurrent/topology.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/concurrent/condition.hpp"
#include "y/concurrent/thread/agent.hpp"

#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

      
        typedef Memory::Wad<Agent,Memory::Dyadic> Agency;

        class Threads : public Agency
        {
        public:
            explicit Threads(const Topology &);
            virtual ~Threads() noexcept;

            Mutex     access;

        protected:
            const size_t  level;
            Agent * const agent;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Threads);
            friend class Agent;

            size_t        done_;
            Condition     waitCV;
            Condition     doneCV;


            void          mainLoop(Agent &agent) noexcept;
            static void   Launch(Threads &threads, Agent &agent) noexcept;
            void          quit() noexcept;
        };

    }

}

#endif
