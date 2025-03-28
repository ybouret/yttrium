
//! \file


#ifndef Y_Chemical_Banks_Included
#define Y_Chemical_Banks_Included 1

#include "y/chemical/plexus/equalizer/cursor.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Shared Banks for Equalizer
        //
        //
        //______________________________________________________________________
        class EqzBanks
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit EqzBanks();          //!< setup
            virtual ~EqzBanks() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SBank  sb; //!< for species
            CrBank cb; //!< for cursors

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EqzBanks);
        };

     
    }

}

#endif

