


//! \file

#ifndef Y_Concurrent_Crew_Included
#define Y_Concurrent_Crew_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/topology.hpp"
#include "y/concurrent/threads.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! Crew of multiple threads
        //
        //
        //______________________________________________________________________
        class Crew : public Loop, public Threads
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            class Code;
            static const char * const CallSign; //!< Concurrent::Crew

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Crew(const Topology &); //!< setup from topology
            virtual ~Crew() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const char *   callSign()                const noexcept; //!< CallSign
            virtual size_t         size()                    const noexcept; //!< number of internal threads
            virtual ConstType &    operator[](const size_t)  const noexcept; //!< access
            virtual void           run1(Kernel &)                  noexcept; //!< dispatch/run kernel for every thread
            virtual Lockable &     sync()                          noexcept; //!< shared sync
            virtual const Thread & getThread(const size_t i) const noexcept; //!< getThread(1..size)
            virtual size_t         numThreads()              const noexcept; //!< size


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Crew);
            Code *code;


        };

    }

}

#endif
