//! \file

#ifndef Y_Field3D_Included
#define Y_Field3D_Included 1

#include "y/field/2d.hpp"
#include "y/field/layout/3d.hpp"
#include "y/memory/embedding/trio.hpp"

namespace Yttrium
{
    namespace Field
    {
#if 0
        //! Generic 3D Sub-Field
        template <size_t NSUB, typename T>
        class Sub3D : public Interface, public Layout3D
        {
        public:
            Y_ARGS_DECL(T,Type);                             //!< aliases
            typedef MetaKeyWith<NSUB>          SelfMetaKey;  //!< alias
            typedef Memory::EmbeddingTrio      SelfPattern;  //!< alias
            typedef Memory::Embedded           SelfAcquire;  //!< alias
            typedef Sub1D<NSUB+2,Type>         RowType;      //!< alias
            typedef Sub2D<NSUB+1,Type>         SliceType;    //!< alias
            typedef MemoryBuilder<SliceType>   SelfBuilder;  //!< alias

            template <typename LABEL>
            explicit Sub3D(const LABEL       & label,
                           const Layout3D    & layout,
                           Memory::Allocator & alloc) :
            Interface(),
            Layout3D(layout),
            metaKey(label),
            in2d(lower.xy(),upper.xy()),
            slc( 0 ),
            row( 0 ),
            ptr( 0 ),
            motif(slc,width.y,row,width.y*width.z,ptr,items),
            owned(motif,alloc),
            inner()
            {
            }

            const SelfMetaKey metaKey;
            Layout2D          in2d;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub3D);
            SliceType   *slc;
            RowType     *row;
            MutableType *ptr;
            SelfPattern  motif;
            SelfAcquire  owned;
            SelfBuilder  inner;
        };
#endif
        
    }



}

#endif


