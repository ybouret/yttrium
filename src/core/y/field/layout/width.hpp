
//! \file

#ifndef Y_Field_Layout_Width_Included
#define Y_Field_Layout_Width_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Field
    {
        template <typename COORD> class LayoutWidth
        {
        public:
            inline virtual ~LayoutWidth() noexcept { Y_STATIC_ZVAR( Coerce(width) ); }
            inline explicit LayoutWidth(const COORD       &dummy) noexcept : width(dummy) {}
            inline explicit LayoutWidth(const LayoutWidth &other) noexcept : width(other.width) {}

            const COORD width;

        private:
            Y_DISABLE_ASSIGN(LayoutWidth);
        };
    }

}
#endif

