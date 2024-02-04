
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
        protected:
            //! dummy setu;
            inline explicit LayoutWidth(const COORD &w, const COORD &s) noexcept : width(w), shift(s)  {}

            //! copy
            inline explicit LayoutWidth(const LayoutWidth &other) noexcept : width(other.width), shift(other.shift) {}

        public:
            //! cleanup
            inline virtual ~LayoutWidth() noexcept {
                Y_STATIC_ZVAR( Coerce(width) );
                Y_STATIC_ZVAR( Coerce(shift) );
            }


            const COORD width; //!< the width
            const COORD shift; //!< shift per dimension

        private:
            Y_DISABLE_ASSIGN(LayoutWidth);
        };
    }

}
#endif

