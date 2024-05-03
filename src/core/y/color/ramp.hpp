//! \file

#ifndef Y_Color_Ramp_Included
#define Y_Color_Ramp_Included 1

#include "y/color/rgba.hpp"

namespace Yttrium
{
    namespace Color
    {
        //______________________________________________________________________
        //
        //
        //! internal color for Ramps
        //
        //______________________________________________________________________
        typedef RGBA<uint8_t> RampColor;

        //______________________________________________________________________
        //
        //
        //
        //! Color Ramp interface
        //
        //
        //______________________________________________________________________
        class Ramp
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! return a color for x in [0:1]
            virtual RampColor operator()(const float x) const noexcept = 0;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Ramp() noexcept; //!< cleanup
        protected:
            explicit Ramp() noexcept; //!< setup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ramp);
        };



    }
}

#endif
