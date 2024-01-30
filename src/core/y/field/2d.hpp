
//! \file

#ifndef Y_Field2D_Included
#define Y_Field2D_Included 1

#include "y/field/1d.hpp"
#include "y/mkl/v2d.hpp"
#include "y/memory/embed.hpp"

namespace Yttrium
{
    namespace Field
    {

        typedef V2D<unit_t>     Coord2D;
        typedef Layout<Coord2D> Layout2D;

        template <typename T>
        class In2D : public Layout2D
        {
        public:
            typedef Layout2D LayoutType;
            typedef Layout1D RowLayout;
            typedef In1D<T>  RowType;

            explicit In2D(const LayoutType  layout,
                          Memory::Allocator &memmgr) :
            LayoutType(layout)
            {
            }
            

            virtual ~In2D() noexcept
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
        };

    }

}

#endif

