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
            typedef MemoryBuilder<RowType>     SelfBuilder;  //!< alias

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sub3D);
            
        };

    }



}

#endif


