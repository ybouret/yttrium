

//! \file


#ifndef Y_Chemical_Latchable_Included
#define Y_Chemical_Latchable_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! latching objects
        //
        //
        //______________________________________________________________________
        class Latchable
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Latchable() noexcept; //!< setup unlatched
            virtual ~Latchable() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void     latch()     noexcept; //!< set latch

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const bool latched; //!< state
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Latchable);
        };
    }

}

#endif
