
#include "y/ink/ops/block/median.hpp"

#include "y/utest/run.hpp"

#include "y/ink/image/codecs.hpp"
#include "y/color/channels.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/rgb/x11.hpp"
#include "y/color/conv.hpp"

using namespace Yttrium;
using namespace Ink;


Y_UTEST(median)
{
    Slabs  par( InParallel );
    Codec &IMG = Codecs::Std();

    if( argc > 1 )
    {
        const Pixmap<RGBA>    img4 = IMG.load(argv[1],0);

        {
            Pixmap<RGBA> tgt4(img4.w,img4.h);
            Median<3,3>::Call(par,tgt4,img4);
            IMG.save(tgt4, "median3x3-rgba.png", 0);
            Median<5,5>::Call(par,tgt4,img4);
            IMG.save(tgt4, "median5x5-rgba.png", 0);
        }

    }


}
Y_UDONE()
