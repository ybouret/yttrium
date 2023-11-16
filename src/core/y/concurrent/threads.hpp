
//! \file

#ifndef Y_Threads_Included
#define Y_Threads_Included 1


#include "y/concurrent/topology.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        class Threads
        {
        public:
            explicit Threads(const Topology &);
            virtual ~Threads() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Threads);
        };

    }

}

#endif
