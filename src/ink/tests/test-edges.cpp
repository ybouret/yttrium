#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/ink/ops/gradient/map.hpp"
#include "y/ink/ops/gradient/thinner.hpp"
#include "y/ink/ops/block/dilate.hpp"
#include "y/ink/ops/block/erode.hpp"
#include "y/ink/ops/blur.hpp"

#include "y/ink/ops/edges.hpp"

#include "y/color/grayscale.hpp"
#include "y/color/ramp/black-and-white.hpp"
#include "y/color/ramp/cold-to-hot.hpp"
#include "y/color/ramp/dark-to-bright.hpp"

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
    const Color::DarkToBright             cr;
    const Color::Map8                     cm8(bw);
    const Blur<float>                     blur(1.4f);

    if(argc>1)
    {
        std::cerr << "Loading " << argv[1] << std::endl;
        const Pixmap<RGBA>  img = IMG.load(argv[1],0);
        std::cerr << "Converting to GrayScale" << std::endl;
        Pixmap<float>       pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        Pixmap<float>       blr(par,Pixmap<float>::Id,pxf);
        GradientMap<float>  gmap(img.w,img.h);
        GradientThinner     thin(img.w,img.h);

        blur(par,blr,pxf);
        //blr.ld(par, Pixmap<float>::Id, pxf);

        // compute gradient intensity/direction
        std::cerr << "Computing gradient" << std::endl;
        gmap(par,scharr5,pxf);
        std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;


        // keep only maxima of gradient and double thresholding
        std::cerr << "Thinning" << std::endl;
        thin(par,hist,gmap);

        std::cerr << "Saving..." << std::endl;
        IMG.save(img, "img1-data.png", 0);
        IMG.save(pxf, "img2-grey.png", 0, par, bw);
        IMG.save(blr, "img3-blur.png", 0, par, bw);
        {
            const Color::FlexibleRamp gr(cr, gmap.nmin, gmap.nmax);
            IMG.save(gmap.intensity, "img4-gmap.png", 0, par, gr);
        }
        IMG.save(thin, "img5-thin.png", 0, par, cm8);

        Pixmap<size_t> labels(img.w,img.h);
        Edges          edges;

        edges(par,labels,thin,Edge::Connect8);

        IMG.save(thin, "img6-thin.png", 0, par, cm8);



    }
}
Y_UDONE()

