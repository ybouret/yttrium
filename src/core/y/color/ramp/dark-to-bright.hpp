

//! \file

#ifndef Y_Color_Ramp_Dark_To_Bright_Included
#define Y_Color_Ramp_Dark_To_Bright_Included 1

#include "y/color/ramp/gradation.hpp"

namespace Yttrium
{
    namespace Color
    {

        //______________________________________________________________________
        //
        //
        //! DarkToBright gradiation
        //
        //______________________________________________________________________
        class DarkToBright : public Gradation
        {
        public:
            explicit DarkToBright() noexcept; //!< setup
            virtual ~DarkToBright() noexcept; //!< cleanup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DarkToBright);
        };
    }

}

#endif


