
//! \file

#ifndef Y_Ordered_Core_Dire_Included
#define Y_Ordered_Core_Dire_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! Core Dire to old CallSign
        //
        //
        //______________________________________________________________________
        class Dire
        {
        public:
            static const char * const CallSign; //!< "Dire"
            virtual ~Dire() noexcept;           //!< cleanup
            explicit Dire() noexcept;           //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dire);
        };
    }

}



#endif
