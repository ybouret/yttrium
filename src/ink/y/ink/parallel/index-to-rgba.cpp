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
            slab.load(target,color);
        }

        void IndexToRGBA:: Load(Pixmap<RGBA> &target,
                                Slabs        &slabs,
                                Random::Bits &ran)
        {
            slabs(Mark,target,ran);
        }
    }

}


