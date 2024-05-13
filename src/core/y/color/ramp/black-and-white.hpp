
//! \file

#ifndef Y_Color_Ramp_Black_And_White_Included
#define Y_Color_Ramp_Black_And_White_Included 1

#include "y/color/ramp/gradation.hpp"

namespace Yttrium
{
    namespace Color
    {

        //______________________________________________________________________
        //
        //
        //! Black To White Gradation
        //
        //______________________________________________________________________
        class BlackAndWhite : public Gradation
        {
        public:
            explicit BlackAndWhite() noexcept; //!< setup
            virtual ~BlackAndWhite() noexcept; //!< cleanup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(BlackAndWhite);
        };
    }

}

#endif


