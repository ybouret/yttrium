
//! \file

#ifndef Y_Field2D_Included
#define Y_Field2D_Included 1

#include "y/field/1d.hpp"
#include "y/mkl/v2d.hpp"

namespace Yttrium
{
    namespace Field
    {
        typedef V2D<unit_t>     Coord2D;
        typedef Layout<Coord2D> Layout2D;
#if 0


        
        template <typename T>
        class In2D : public Layout2D
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef Layout2D LayoutType;
            typedef Layout1D RowLayout;
            typedef In1D<T>  RowType;

            inline explicit In2D(const LayoutType  layout,
                          Memory::Allocator &memmgr):
            LayoutType(layout)
            {
            }

            inline const RowType & operator[](const unit_t j) const noexcept
            {
                assert(0!=row); assert(j>=lower.y); assert(j<=upper.y);
                return row[j];
            }

            inline  RowType & operator[](const unit_t j) noexcept
            {
                assert(0!=row); assert(j>=lower.y); assert(j<=upper.y);
                return row[j];
            }


            inline virtual ~In2D() noexcept
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
            RowType     *row;
            MutableType *ptr;
        };
#endif

        

    }

}

#endif

