

//! \file

#ifndef Y_Ink_Block_Erode_Included
#define Y_Ink_Block_Erode_Included 1

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
        //! Replace by local minimum
        //
        //
        //______________________________________________________________________
        template <size_t W, size_t H>
        struct Erode
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
                block(slabs,target, BlockOps<T>:: template Minimum<BlockType::N>,source);
            }

        };


    }

}

#endif

