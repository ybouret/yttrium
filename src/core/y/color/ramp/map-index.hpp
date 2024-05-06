
//! \file

#ifndef Y_Color_Ramp_MapIndex_Included
#define Y_Color_Ramp_MapIndex_Included 1

#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Color
    {

        class MapIndex
        {
        public:
            //! setup with persisent colors
            explicit MapIndex(const RampColor *entry,
                              const size_t     count) noexcept;

            //! cleanup
            virtual ~MapIndex() noexcept;

            RampColor operator()(const size_t indx) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MapIndex);
            const RampColor * const head;
            const size_t            size;
        };

    }

}

#endif

