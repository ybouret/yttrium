#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/ink/ops/gradient/map.hpp"
#include "y/ink/ops/gradient/thinner.hpp"
#include "y/ink/ops/block/dilate.hpp"
#include "y/ink/ops/block/erode.hpp"


#include "y/color/grayscale.hpp"
#include "y/color/ramp/black-and-white.hpp"
#include "y/color/ramp/cold-to-hot.hpp"
#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/map8.hpp"

using namespace Yttrium;
using namespace Ink;

Y_UTEST(edges)
{
    Slabs  par( InParallel );
    Codec &IMG = Codecs::Std();
    GradientFilters<float,Crux::Scharr5>  scharr5;
    Histogram                             hist;
    const Color::BlackAndWhite            bw;
    const Color::ColdToHot                cr;
    const Color::Map8                     cm8(bw);

    if(argc>1)
    {
        const Pixmap<RGBA>  img = IMG.load(argv[1],0);                       IMG.save(img, "img.png", 0);
        Pixmap<float>       pxf(par,Color::GrayScale::Pack<float,RGBA>,img); IMG.save(pxf, "pxf.png", 0, par, bw);
        GradientMap<float>  gmap(img.w,img.h);
        GradientThinner     thin(img.w,img.h);

        gmap(par,scharr5,pxf);
        std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;
        {
            const Color::FlexibleRamp gr(cr, gmap.nmin, gmap.nmax);
            IMG.save(gmap.intensity, "gmap.png", 0, par, gr);
        }

        thin(par,hist,gmap);
        IMG.save(thin, "thin.png", 0, par, cm8);
        

#if 0
        Pixmap<uint8_t> dilated(img.w,img.h);
        Dilate<3,3>::Call(par,dilated,thin);
        IMG.save(dilated, "dilated.png", 0, par, cm8);
        Pixmap<uint8_t> eroded(img.w,img.h);
        Erode<3,3>::Call(par,eroded,dilated);
        IMG.save(eroded, "eroded.png", 0, par, cm8);
#endif


    }
}
Y_UDONE()

