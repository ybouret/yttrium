
//! \file

#ifndef Y_Ink_Area_Included
#define Y_Ink_Area_Included 1

#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //! Physical Coordinate
        //
        //______________________________________________________________________
        typedef V2D<unit_t> Coord;

        //______________________________________________________________________
        //
        //
        //
        //!  Basic Area
        //
        //
        //______________________________________________________________________
        class Area
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Area(const unit_t W, const unit_t H) noexcept; //!< setup 0+W,0+H
            virtual ~Area()                      noexcept; //!< cleanup
            Area(const Area &)                   noexcept; //!< copy

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Coord lower()                        const noexcept; //!< (x,y)
            Coord upper()                        const noexcept; //!< (xt,yt)
            bool  hasSameSizesThan(const Area &) const noexcept; //!< check same w/h

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unit_t x;  //!< starting x
            const unit_t y;  //!< starting y
            const unit_t w;  //!< width
            const unit_t h;  //!< height
            const size_t n;  //!< number of items
            const unit_t xt; //!< x top
            const unit_t yt; //!< y top

        private:
            Y_DISABLE_ASSIGN(Area);

        };

    }
}

#endif
