
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



#if 0
        template <typename T>
        class In2D : public Layout2D
        {
        public:
            Y_ARGS_DECL(T,Type);
            typedef Layout2D LayoutType;
            typedef Layout1D RowLayout;
            typedef In1D<T>  RowType;

            explicit In2D(const LayoutType  layout,
                          Memory::Allocator &memmgr) :
            LayoutType(layout),
            row(0),
            input(0),
            bytes(0),
            entry(0),
            alloc(0),
            built(0)
            {
                setup(memmgr);
            }


            virtual ~In2D() noexcept { clear(); }

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


        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
            RowType           *row;
            MutableType       *input;
            size_t             bytes;
            void              *entry;
            Memory::Allocator *alloc;
            size_t             built;

            inline void setup( Memory::Allocator &memmgr )
            {
                // main allocation
                {
                    Memory::Embed emb[] =
                    {
                        Memory::Embed(row,width.y),
                        Memory::Embed(input,items)
                    };

                    entry = Memory::Embed::Build(emb,sizeof(emb)/sizeof(emb[0]),memmgr,bytes);
                }

                // tuning
                alloc = &memmgr;
                row  -= lower.y;

                // no throw
                link();

                // may throw
                try {
                    while(built<items) {
                        new (input+built) MutableType();
                        ++built;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }

            }

            inline void clear()
            {
                for(unit_t j=lower.y; j<=upper.y; ++j)
                {
                    Memory::OutOfReach::Naught( &row[j] );
                }

                row = 0;
                if(0!=alloc)
                {
                    while(built>0)
                        Memory::OutOfReach::Naught( &input[--built] );
                    input = 0;
                    alloc->release(entry,bytes);
                    alloc = 0;
                }
            }

            inline void link() noexcept
            {
                const RowLayout l(lower.x,upper.x);
                MutableType    *p = input;
                for(unit_t j=lower.y; j<=upper.y; ++j, p+=l.width)
                    new (row+j) RowType(l,p);

            }

        };
#endif

    }

}

#endif

