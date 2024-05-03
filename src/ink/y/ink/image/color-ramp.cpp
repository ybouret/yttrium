#include "y/ink/image/codecs.hpp"
#include "y/color/ramp.hpp"

namespace Yttrium
{
    namespace Ink
    {
        void Codecs:: renderRamp(const String      &fileName,
                                 const Color::Ramp &ramp,
                                 const unit_t       W,
                                 const unit_t       H) const
        {
            Image img(W,H); assert(img.w-1==img.xt);
            const double den = double(img.xt);
            for(unit_t x=0;x<img.w;++x)
            {
                const RGBA c = ramp(x/den);
                for(unit_t y=0;y<img.h;++y)
                    img[y][x] = c;
            }
            save(img,fileName,0);
        }

    }

}

