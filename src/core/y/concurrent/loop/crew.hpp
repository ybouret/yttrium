


//! \file

#ifndef Y_Concurrent_Crew_Included
#define Y_Concurrent_Crew_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/concurrent/topology.hpp"


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
        class Crew : public Loop
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
            // Methods
            //
            //__________________________________________________________________
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual size_t       size()                   const noexcept; //!< number of internal threads
            virtual ConstType &  operator[](const size_t) const noexcept; //!< access
            virtual void         operator()(Kernel &)           noexcept; //!< dispatch/run kernel for every thread

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Crew);
            Code *code;


        };

    }

}

#endif
