

//! \file

#ifndef Y_Field_Layout_In2D_Included
#define Y_Field_Layout_In2D_Included 1

#include "y/field/layout.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{

    namespace Field
    {
        typedef V2D<unit_t>     Coord2D;  //!< alias
        typedef Layout<Coord2D> Layout2D; //!< alias
    }

}

#endif

