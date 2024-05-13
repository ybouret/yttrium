

//! \file

#ifndef Y_Color_Ramp_Cold_To_Hot_Included
#define Y_Color_Ramp_Cold_To_Hot_Included 1

#include "y/color/ramp/gradation.hpp"

namespace Yttrium
{
    namespace Color
    {
        //______________________________________________________________________
        //
        //
        //! Cold To Hot Gradation
        //
        //______________________________________________________________________
        class ColdToHot : public Gradation
        {
        public:
            explicit ColdToHot() noexcept; //!< setup
            virtual ~ColdToHot() noexcept; //!< cleanup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ColdToHot);
        };
    }

}

#endif


