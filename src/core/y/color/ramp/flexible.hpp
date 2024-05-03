
//! \file

#ifndef Y_Color_Ramp_Flexible_Included
#define Y_Color_Ramp_Flexible_Included 1

#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Color
    {

        //______________________________________________________________________
        //
        //
        //
        //! Gradation over an arbitratry number of colors
        //
        //
        //______________________________________________________________________
        class FlexibleRamp : public Ramp
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~FlexibleRamp() noexcept;
            explicit FlexibleRamp(const Ramp &, const float fmin, const float fmax);

            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            virtual RampColor operator()(const float x) const noexcept;
            
        private:
            const Ramp &ramp;
            const float vmin;
            const float vmax;
            const float delv;
            Y_DISABLE_COPY_AND_ASSIGN(FlexibleRamp);
        };

    }

}

#endif

