
//! \file

#ifndef Y_Field_Layout_In4D_Included
#define Y_Field_Layout_In4D_Included 1

#include "y/field/layout.hpp"
#include "y/mkl/v4d.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{

    namespace Field
    {
        typedef V4D<unit_t>             Coord4D;  //!< alias
        typedef V4D<size_t>             Count4D;  //!< alias
        typedef Layout<Coord4D,Count4D> Layout4D; //!< alias
        typedef ArcPtr<const Layout4D>  Format4D; //!< alias
    }

}

#endif

