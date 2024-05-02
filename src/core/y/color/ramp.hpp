//! \file

#ifndef Y_Color_Ramp_Included
#define Y_Color_Ramp_Included 1

#include "y/color/rgba.hpp"

namespace Yttrium
{
    namespace Color
    {
        typedef RGBA<uint8_t> RampColor;

        class Ramp
        {
        public:
            virtual ~Ramp() noexcept;
            virtual RampColor operator()(const float x) const noexcept = 0;

        protected:
            explicit Ramp() noexcept;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ramp);
        };



    }
}

#endif
