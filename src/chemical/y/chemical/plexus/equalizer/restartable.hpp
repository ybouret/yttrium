
//! \file


#ifndef Y_Chemical_Restartable_Included
#define Y_Chemical_Restartable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! interface for equalizing objects
        //
        //
        //______________________________________________________________________
        class Restartable
        {
        public:
            static const unsigned     Width = 22; //!< for display
            static const char * const None;       //!< "none"

        protected:
            explicit Restartable() noexcept; //!< setup

        public:
            virtual ~Restartable() noexcept; //!< cleanup

            virtual void restart() noexcept = 0; //!< should reset data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Restartable);
        };

    }

}

#endif

