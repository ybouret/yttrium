//! \file

#ifndef Y_Chemical_Banks_Included
#define Y_Chemical_Banks_Included 1

#include "y/chemical/plexus/limits.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Boundary/Species bank for Coop lists and build Fences
        //
        //
        //______________________________________________________________________
        class Banks
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Banks();
            ~Banks() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            BBank hb; //!< heavy boundaries
            SBank ls; //!< light species

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Banks);
        };

    }

}

#endif
