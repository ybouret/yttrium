


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
            static const char * const CallSign;

            explicit Crew(const Topology &);
            virtual ~Crew() noexcept;

            virtual const char * callSign() const noexcept;
            virtual size_t       size()     const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Crew);
            virtual ConstType & operator[](const size_t) const noexcept;
            const Agent * const item; //!< offset Threads agents
        };

    }

}

#endif
