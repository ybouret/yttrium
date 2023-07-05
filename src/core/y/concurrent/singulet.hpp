//! \file

#ifndef Y_Concurrent_Singulet_Included
#define Y_Concurrent_Singulet_Included 1

#include "y/concurrent/mutex.hpp"
#include "y/system/at-exit.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! base class for Singleton
        //
        //
        //______________________________________________________________________
        class Singulet
        {
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
        protected:
            explicit Singulet() noexcept;

        public:
            virtual ~Singulet() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual const char *      callSign() const noexcept = 0; //!< forward CallSign
            virtual AtExit::Longevity lifeTime() const noexcept = 0; //!< forward LifeTime

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            static bool Verbose; //!< operations verbosity

        protected:
            //! check valid LifeTime, throw on error
            static void CheckLifeTime(const char             *whose,
                                      const AtExit::Longevity which);

            //! check valid address, critical on error
            static void CheckInstance(const char *whose,
                                      void       *where);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Singulet);
        };
    }
}

#endif

