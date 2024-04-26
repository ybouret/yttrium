#include "y/ink/parallel/index-to-rgba.hpp"

namespace Yttrium
{
    namespace Ink
    {
        void IndexToRGBA:: Mark(Ink::Slab          &slab,
                                Pixmap<RGBA>       &target,
                                Random::Bits       &ran) noexcept
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

        void IndexToRGBA:: Load(Pixmap<RGBA> &target,
                                Slabs        &slabs,
                                Random::Bits &ran)
        {
            slabs(Mark,target,ran);
        }
    }

}


