
//! \file


#ifndef Y_Chemical_Banks_Included
#define Y_Chemical_Banks_Included 1

#include "y/chemical/plexus/equalizing/cursor.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Equalizer
        {
            //__________________________________________________________________
            //
            //
            //
            //! Shared Banks for Equalizer
            //
            //
            //__________________________________________________________________
            class Banks
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Banks();          //!< setup
                virtual ~Banks() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                SBank  sb; //!< for species
                CrBank cb; //!< for cursors
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Banks);
            };
        }

     
    }

}

#endif

