//! \file

#ifndef Y_Field_Layout_In1D_Included
#define Y_Field_Layout_In1D_Included 1

#include "y/field/layout.hpp"
#include "y/ptr/arc.hpp"
namespace Yttrium
{

    namespace Field
    {
        typedef unit_t                  Coord1D;  //!< alias
        typedef size_t                  Count1D;  //!< alias
        typedef Layout<Coord1D,Count1D> Layout1D; //!< alias
        typedef ArcPtr<const Layout1D>  Format1D; //!< alias;

    }

}

#endif

