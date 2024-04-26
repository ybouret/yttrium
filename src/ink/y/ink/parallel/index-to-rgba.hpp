
//! \file

#ifndef Y_Ink_Parallel_Indexer_Included
#define Y_Ink_Parallel_Indexer_Included 1

#include "y/ink/parallel/slabs.hpp"
#include "y/ink/pixmap.hpp"
#include "y/ink/types.hpp"
#include "y/random/bits.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Helper to load each slab with a different random color
        /**
         This is also an example of parallel operation
         */
        //
        //______________________________________________________________________
        struct IndexToRGBA
        {
            //! load slab with the same random color
            static void Mark(Slab          &slab,
                             Pixmap<RGBA>  &target,
                             Random::Bits  &ran) noexcept;

            //! mark each slab of target
            static void Load(Pixmap<RGBA> &target,
                             Slabs        &slabs,
                             Random::Bits &ran);

        };

    }

}

#endif
