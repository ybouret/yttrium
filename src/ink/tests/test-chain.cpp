
#include "y/ink/ops/block/chain.hpp"
#include "y/ink/ops/block/erode.hpp"
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


Y_UTEST(chain)
{
    Slabs  par( InParallel );
    Codec &IMG = Codecs::Std();

    if( argc > 1 )
    {
        const Pixmap<RGBA>    img4 = IMG.load(argv[1],0);
        Pixmap<RGBA>          tgt4(img4.w,img4.h);
        Pixmap<RGBA>          tmp4(img4.w,img4.h);

        IMG.save(img4,"image.png", 0);

        Chain2< Dilate<3,3>, Erode<3,3> >::Call(par, tgt4, tmp4, img4);
        IMG.save(tgt4, "close3x3-rgba.png", 0);

        Chain2< Erode<3,3>, Dilate<3,3> >::Call(par, tgt4, tmp4, img4);
        IMG.save(tgt4, "open3x3-rgba.png", 0);

#if 0
        {
            Pixmap<RGBA> tgt4(img4.w,img4.h);
            Mean<3,3>::Call(par,tgt4,img4);
            IMG.save(tgt4, "mean3x3-rgba.png", 0);
            Mean<5,5>::Call(par,tgt4,img4);
            IMG.save(tgt4, "mean5x5-rgba.png", 0);
        }
#endif

    }


}
Y_UDONE()
