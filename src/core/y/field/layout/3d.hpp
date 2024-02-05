//! \file

#ifndef Y_Field_Layout_In3D_Included
#define Y_Field_Layout_In3D_Included 1

#include "y/field/layout.hpp"
#include "y/mkl/v3d.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{

    namespace Field
    {
        typedef V3D<unit_t>             Coord3D;  //!< alias
        typedef V3D<size_t>             Count3D;  //!< alias
        typedef Layout<Coord3D,Count3D> Layout3D_; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! 3D Layout
        //
        //
        //______________________________________________________________________
        class Layout3D : public Layout3D_
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
           
            //! setup
            explicit Layout3D(const Coord3D lo, const Coord3D up) noexcept;

            //! setup
            template <typename SUPER> inline
            explicit Layout3D(const SubLayout_ &_, const SUPER &super) noexcept :
            Layout3D_(_,super),
            nr(width.y*width.z)
            {
            }

            //! cleanup
            virtual ~Layout3D() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t numSlices() const noexcept; //!< width.z
            size_t numRows()   const noexcept; //!< precomputed width.z * width.y

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layout3D);
            const size_t nr; //!< num rows in volume
        };

        typedef ArcPtr<const Layout3D>  Format3D; //!< alias
    }

}

#endif

