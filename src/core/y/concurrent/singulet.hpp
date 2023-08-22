//! \file

#ifndef Y_Concurrent_Singulet_Included
#define Y_Concurrent_Singulet_Included 1

#include "y/system/at-exit.hpp"
#include "y/type/identifiable.hpp"

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
        class Singulet : public Identifiable
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Singulet() noexcept; //!< setup

        public:
            virtual ~Singulet() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual AtExit::Longevity lifeTime() const noexcept = 0; //!< forward LifeTime

            //! show if Verbose
            static void OnInitDisplay(const char             *name,
                                      const AtExit::Longevity last) noexcept;

            //! show if Verbose
            static void OnQuitDisplay(const char             *name,
                                      const AtExit::Longevity last) noexcept;
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            static bool   Verbose; //!< operations verbosity
            static size_t Width;   //!< for callSign alignment, initally 32
            
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

