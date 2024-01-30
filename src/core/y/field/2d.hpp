
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
            Y_ARGS_DECL(T,Type);
            typedef Layout2D LayoutType;
            typedef Layout1D RowLayout;
            typedef In1D<T>  RowType;

            explicit In2D(const LayoutType  layout,
                          Memory::Allocator &memmgr) :
            LayoutType(layout),
            row(0),
            ptr(0),
            bytes(0),
            entry(0),
            alloc(0)
            {
                setup(memmgr);
            }


            virtual ~In2D() noexcept
            {
                clear();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
            RowType           *row;
            MutableType       *ptr;
            size_t             bytes;
            void              *entry;
            Memory::Allocator *alloc;

            inline void setup( Memory::Allocator &memmgr )
            {
                Memory::Embed emb[] =
                {
                    Memory::Embed(row,width.y),
                    Memory::Embed(ptr,items)
                };
                Core::Display(std::cerr, emb, 2) << std::endl;

                alloc = &memmgr;
                entry = Memory::Embed::Build(emb,sizeof(emb)/sizeof(emb[0]),memmgr,bytes);
            }

            inline void clear()
            {
                if(0!=alloc)
                {
                    alloc->release(entry,bytes);
                    alloc = 0;
                }
            }

        };

    }

}

#endif

