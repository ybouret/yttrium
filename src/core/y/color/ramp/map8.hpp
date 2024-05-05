
//! \file

#ifndef Y_Color_Ramp_Map8_Included
#define Y_Color_Ramp_Map8_Included 1

#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Color
    {
        //______________________________________________________________________
        //
        //
        //
        //! Map 8bits to a color using pre-defined Ramp
        //
        //
        //______________________________________________________________________
        class Map8
        {
        public:
            explicit Map8(const Ramp &user)           noexcept; //!< setup
            virtual ~Map8()                           noexcept; //!< cleanup
            RampColor operator()(const uint8_t) const noexcept; //!< convert

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Map8);
            const Ramp &ramp;
        };
    }

}

#endif

