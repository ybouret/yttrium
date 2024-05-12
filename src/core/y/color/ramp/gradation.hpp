
//! \file

#ifndef Y_Color_Gradation_Included
#define Y_Color_Gradation_Included 1

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
        class Gradation : public Ramp
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            
            //! setup with persisent colors
            explicit Gradation(const RampColor *entry,
                               const size_t     count) noexcept;

            //! cleanup
            virtual ~Gradation() noexcept;

            //__________________________________________________________________
            //
            //
            // interface
            //
            //__________________________________________________________________
            virtual RampColor operator()(const float x) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradation);
            const RampColor * const head;
            const size_t            size;
            const size_t            imax;
            const RampColor * const tail;
        };

#define Y_COLOR_GRADATION(ARR) ARR, sizeof(ARR)/sizeof(ARR[0])

    }

}

#endif
