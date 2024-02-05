
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
        typedef Layout<Coord4D,Count4D> Layout4D_; //!< alias

        class Layout4D : public Layout4D_
        {
        public:
            explicit Layout4D(const Coord4D lo, const Coord4D up) noexcept;
            virtual ~Layout4D() noexcept;

            size_t numSlices() const noexcept; //!< precompute  width.z * width.w
            size_t numRows()   const noexcept; //!< precomputed  width.y * numSlices()

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layout4D);
            const size_t ns; //!< num slices: width.w * width.z
            const size_t nr; //!< ns * width.y
        };
        
        typedef ArcPtr<const Layout4D>  Format4D; //!< alias
    }

}

#endif

