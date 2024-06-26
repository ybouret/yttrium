
//! \file

#ifndef Y_Color_Ramp_MapIndex_Included
#define Y_Color_Ramp_MapIndex_Included 1

#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Color
    {

        //______________________________________________________________________
        //
        //
        //
        //! convert indices to background or cyclic color
        //
        //
        //______________________________________________________________________
        class MapIndex
        {
        public:
            //! setup with count>=2 persisent colors
            explicit MapIndex(const RampColor *entry,
                              const size_t     count) noexcept;

            //! cleanup
            virtual ~MapIndex() noexcept;

            //! return background or colors otherwise
            RampColor operator()(const size_t indx) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MapIndex);
            const RampColor * const head; //!< head[0..size-1]
            const size_t            size; //!< size>1
            const size_t            smod; //!< size-1
        };

    }

}

#endif

