#include "y/ink/ops/block/dilate.hpp"

#include "y/utest/run.hpp"

#include "y/ink/image/codecs.hpp"
#include "y/color/channels.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/rgb/x11.hpp"
#include "y/color/conv.hpp"

using namespace Yttrium;
using namespace Ink;


Y_UTEST(dilate)
{
    Slabs  par( InParallel );
    Codec &IMG = Codecs::Std();


    if( argc > 1 )
    {
        const Pixmap<RGBA>    img4 = IMG.load(argv[1],0);
        
        {
            Pixmap<RGBA> tgt4(img4.w,img4.h);
            Dilate<3,3>::Call(par,tgt4,img4);
            IMG.save(tgt4, "dilate3x3-rgba.png", 0);
            Dilate<5,5>::Call(par,tgt4,img4);
            IMG.save(tgt4, "dilate5x5-rgba.png", 0);
        }

    }


}
Y_UDONE()
