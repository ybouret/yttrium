//! \file

#ifndef Y_Field_Layout_In3D_Included
#define Y_Field_Layout_In3D_Included 1

#include "y/field/layout.hpp"
#include "y/mkl/v3d.hpp"

namespace Yttrium
{

    namespace Field
    {
        typedef V3D<unit_t>             Coord3D;  //!< alias
        typedef V3D<size_t>             Count3D;  //!< alias
        typedef Layout<Coord3D,Count3D> Layout3D; //!< alias
    }

}

#endif

