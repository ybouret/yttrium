
//! \file

#ifndef Y_Color_Gradation_Included
#define Y_Color_Gradation_Included 1

#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Color
    {

        class Gradation : public Ramp
        {
        public:
            virtual ~Gradation() noexcept;
            explicit Gradation(const RampColor *entry,
                               const size_t     count) noexcept;

            virtual RampColor operator()(const float x) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradation);

            const RampColor * const cmin; //!< min color
            const size_t            size; //!< number of colors
            const size_t            imax; //!< size-1
            const RampColor * const cmax; //!< cmin[imax]
        };

    }

}

#endif
