

//! \file


#ifndef  Y_Locking_Regular_Included
#define  Y_Locking_Regular_Included 1

#include "y/concurrent/mutex.hpp"


namespace Yttrium
{

    namespace Locking
    {
        //______________________________________________________________________
        //
        //
        //
        //! Base class to use Singleton based on class-wide mutex
        //
        //
        //______________________________________________________________________
        class Regular
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const bool                IsRegular = true;  //!< alias
            static const bool                IsNucleus = false; //!< alias
            typedef Concurrent::Mutex        MutexType;         //!< alias

            explicit Regular() noexcept;
            virtual ~Regular() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Regular);


        };

    }

}

#endif

