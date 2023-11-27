
//! \file

#ifndef Y_Memory_Buffer_Ops_Included
#define Y_Memory_Buffer_Ops_Included 1

#include "y/memory/buffer/ro.hpp"
#include "y/data/small/light/list.hpp"

namespace Yttrium
{

    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //! Operations on Buffers
        //
        //______________________________________________________________________
        struct BufferOps
        {
            typedef Small::LightNode<ReadOnlyBuffer> BufferNode; //!< alias
            typedef ListOf<BufferNode>               BufferList; //!< alias

            //! find maximum homology within list of buffers
            static size_t Homology(const BufferList &blist) noexcept;
        };

    }
}

#endif

