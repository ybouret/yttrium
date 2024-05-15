
//! \file

#ifndef Y_Ink_Block_Dilate_Included
#define Y_Ink_Block_Dilate_Included 1

#include "y/ink/ops/block.hpp"
#include "y/ink/ops/block/ops.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Replace by local maximum
        //
        //
        //______________________________________________________________________
        template <size_t W, size_t H>
        struct Dilate
        {
            //! alias
            typedef  Block<W,H> BlockType;

            //! call
            template <typename T> static inline
            void Call(Slabs           &slabs,
                      Pixmap<T>       &target,
                      const Pixmap<T> &source)
            {
                const BlockType block;
                block(slabs,target, BlockOps<T>:: template Maximum<BlockType::N>,source);
            }

        };


    }

}

#endif

