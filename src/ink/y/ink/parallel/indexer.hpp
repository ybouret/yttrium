
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

        struct ParallelIndexer
        {
            static inline
            void Load (Ink::Slab          &slab,
                       Pixmap<RGBA>       &target,
                       Random::Bits       &ran)
            {

                Ink::RGBA color;
                {
                    Y_LOCK(slab.sync);
                    color.r = ran.in<uint8_t>(100,255);
                    color.g = ran.in<uint8_t>(100,255);
                    color.b = ran.in<uint8_t>(100,255);
                }
                for(size_t k=slab.count();k>0;--k)
                {
                    const Ink::HSegment    s = slab.hseg[k];
                    PixRow<RGBA>          &r = target[s.y];
                    for(unit_t i=s.w,x=s.x;i>0;--i,++x)
                    {
                        r[x] = color;
                    }
                }
            }
        };

    }

}

#endif
