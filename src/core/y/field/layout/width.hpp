
//! \file

#ifndef Y_Field_Layout_Width_Included
#define Y_Field_Layout_Width_Included 1

#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    namespace Field
    {
        //______________________________________________________________________
        //
        //
        //
        //! Layout Width to get width before Layout
        //
        //
        //______________________________________________________________________
        template <typename COORD> class LayoutWidth
        {
        public:

            inline virtual ~LayoutWidth() noexcept { Y_STATIC_ZVAR( Coerce(width) ); }              //!< cleanup
            inline explicit LayoutWidth(const COORD       &dummy) noexcept : width(dummy) {}        //!< dummy setup
            inline explicit LayoutWidth(const LayoutWidth &other) noexcept : width(other.width) {}  //!< copy

            const COORD width; //!< the width

        private:
            Y_DISABLE_ASSIGN(LayoutWidth);
        };
    }

}
#endif

