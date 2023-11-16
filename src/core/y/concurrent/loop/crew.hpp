


//! \file

#ifndef Y_Concurrent_Crew_Included
#define Y_Concurrent_Crew_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/threads.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        class Crew : public Loop, public Threads
        {
        public:
            explicit Crew(const Topology &);
            virtual ~Crew() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Crew);
            Threads threads;
        };

    }

}

#endif
