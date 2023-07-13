
//! \file


#ifndef  Y_Locking_Nucleus_Included
#define  Y_Locking_Nucleus_Included 1

#include "y/concurrent/mutex.hpp"


namespace Yttrium
{

    namespace Locking
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base class to use Singleton based on system wide mutex
        //
        //
        //______________________________________________________________________
        class Nucleus
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const bool                IsRegular = false; //!< alias
            static const bool                IsNucleus = true;  //!< alias
            typedef Concurrent::NucleusMutex MutexType;         //!< alias

            explicit Nucleus() noexcept;
            virtual ~Nucleus() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Nucleus);


        };

    }

}

#endif

