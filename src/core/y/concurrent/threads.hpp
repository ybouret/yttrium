
//! \file

#ifndef Y_Threads_Included
#define Y_Threads_Included 1


#include "y/concurrent/topology.hpp"
#include "y/concurrent/mutex.hpp"
#include "y/concurrent/condition.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Threads
        {
        public:
            explicit Threads(const Topology &);
            virtual ~Threads() noexcept;

            Mutex     access;
            Condition waitCV;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Threads);

        };

    }

}

#endif
