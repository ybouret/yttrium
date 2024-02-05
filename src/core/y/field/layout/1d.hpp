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
        typedef Layout<Coord1D,Count1D> Layout1D_;

        class Layout1D : public Layout1D_
        {
        public:
            explicit Layout1D(const Coord1D lo, const Coord1D up) noexcept;

            template <typename SUPER> inline
            explicit Layout1D(const SubLayout_ &_, const SUPER &super) noexcept :
            Layout1D_(_,super)
            {
            }

            virtual ~Layout1D() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layout1D);
        };

        typedef ArcPtr<const Layout1D>  Format1D; //!< alias;



    }

}

#endif

