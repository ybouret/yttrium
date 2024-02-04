
//! \file

#ifndef Y_Field2D_Included
#define Y_Field2D_Included 1

#include "y/field/1d.hpp"
#include "y/field/layout/2d.hpp"
#include "y/memory/embedding/pair.hpp"

namespace Yttrium
{
    namespace Field
    {
        
        //! Generic 2D Sub-Field
        template <size_t NSUB, typename T>
        class Sub2D : public Interface, public Layout2D
        {
        public:
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<NSUB>          SelfMetaKey;  //!< alias
            typedef Memory::EmbeddingPair      SelfPattern;  //!< alias
            typedef Memory::Embedded           SelfAcquire;  //!< alias
            typedef Sub1D<NSUB+1,Type>         RowType;      //!< alias
            typedef MemoryBuilder<RowType>     SelfBuilder;  //!< alias


            template <typename LABEL>
            inline explicit Sub2D(const LABEL       & label,
                                  const Layout2D    & layout,
                                  Memory::Allocator & alloc) :
            Interface(),
            Layout2D(layout),
            metaKey(label),
            row(0),
            ptr(0),
            in1d(lower.x,upper.x),
            motif(row,width.y,ptr,items),
            owned(motif,alloc),
            inner(row,width.y, metaKey,lower.y, in1d,ptr)
            {
                row -= lower.y;
            }

            inline virtual ~Sub2D() noexcept { row=0; }
            
            inline virtual const MetaKey & key() const noexcept { return metaKey; }

            inline RowType & operator[](const unit_t j) noexcept
            {
                assert(j>=lower.y); assert(j<=upper.y); return row[j];
            }

            inline const RowType & operator[](const unit_t j) const noexcept
            {
                assert(j>=lower.y); assert(j<=upper.y); return row[j];
            }

            const SelfMetaKey metaKey;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub2D);
            RowType      * row;
            MutableType  * ptr;
            const Layout1D in1d;
            SelfPattern    motif;
            SelfAcquire    owned;
            SelfBuilder    inner;
        };


        template <typename T, typename ALLOCATOR>
        class In2D : public Sub2D<0,T>
        {
        public:
            template <typename LABEL>
            inline explicit In2D(const LABEL    & label,
                                 const Layout2D & layout) :
            Sub2D<0,T>(label,layout, ALLOCATOR::Instance() )
            {
            }


            inline virtual ~In2D() noexcept {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(In2D);
        };

        

    }

}

#endif

