//! \file

#ifndef Y_Field_Layout_In2D_Included
#define Y_Field_Layout_In2D_Included 1

#include "y/field/layout.hpp"
#include "y/mkl/v2d.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{

    namespace Field
    {
        typedef V2D<unit_t>             Coord2D;  //!< alias
        typedef V2D<size_t>             Count2D;  //!< alias
        typedef Layout<Coord2D,Count2D> Layout2D_; //!< alias


        class Layout2D : public Layout2D_
        {
        public:
            explicit Layout2D(const Coord2D lo, const Coord2D up) noexcept;
           
            template <typename SUPER> inline
            explicit Layout2D(const SubLayout_ &_, const SUPER &super) noexcept :
            Layout2D_(_,super)
            {
            }

            virtual ~Layout2D() noexcept;

            size_t numRows() const noexcept; //!< width.y


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layout2D);

        };


        typedef ArcPtr<const Layout2D>  Format2D; //!< alias

    }

}

#endif

