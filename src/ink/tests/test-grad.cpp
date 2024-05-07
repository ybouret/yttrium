

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/ramp/map8.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"
#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"
#include "y/ink/ops/gradient/thinner.hpp"
#include "y/ink/ops/edges.hpp"


#include "y/color/rgb/x11.hpp"
#include "y/color/ramp/map-index.hpp"




using namespace Yttrium;
using namespace Ink;

#include "y/stream/libc/output.hpp"


static inline
void processGrad(Slabs                  &par,
                 GradientMap<float>     &gmap,
                 const Gradient<float>  &grad,
                 GradientThinner        &thin,
                 const Pixmap<float>    &pxf,
                 const Color::Ramp      &cr)
{
    std::cerr << "Apply " << grad.name << std::endl;
    Codec &IMG = Codecs::Location();
    Histogram hist;
    gmap(par,grad,pxf);
    std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;
    thin(par,hist,gmap);
    (std::cerr << "saving..." << std::endl).flush();



    {
        {
            const Color::FlexibleRamp ramp(cr,gmap.nmin,gmap.nmax);
            const String fileName = "grad-" + grad.name + ".png";
            IMG.save(gmap.intensity,fileName, 0, par, ramp);
        }

        {
            Color::RampColor       bwc[2] = { Y_Black, Y_White };
            const Color::Gradation bwg(bwc,2);
            const Color::Map8      ramp(bwg);
            const String fileName = "thin-" + grad.name + ".png";
            IMG.save(thin.force,fileName, 0,par,ramp);
        }

        
        std::cerr << "building edges..." << std::endl;
        Edges          edges;
        Pixmap<size_t> label(pxf.w,pxf.h);
        edges.build(par,label, thin.force, Edge::Connect8);
        {
            Color::RampColor       ecr[] = { Y_White, Y_Blue, Y_Red, Y_Green };
            const Color::MapIndex  icr(ecr,sizeof(ecr)/sizeof(ecr[0]));
            const String fileName = "edge-" + grad.name + ".png";
            IMG.save(label,fileName, 0,par,icr);
        }

#if 0
        {
            const String fileName = "hist-" + grad.name + ".dat";
            hist.save(fileName);
        }

        {
            const String fileName = "sep-" + grad.name + ".dat";
            OutputFile fp(fileName);
            for(unsigned i=0;i<256;++i)
            {
                fp("%g %g\n", double(i), double( hist.separation(i)));
            }
            const uint8_t t = hist.threshold();
            std::cerr << "threshold= " << int(t) << std::endl;
        }
#endif

    }
}

Y_UTEST(grad)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codecs &               IMG = Ink::Codecs::Std();

    static const RGBA Grad[] = {
        RGBA(0x00,0x00,0x00),
        RGBA(0x00,0x00,0xff),
        RGBA(0x00,0xff,0x00),
        RGBA(0xff,0x00,0x00),
        RGBA(0xff,0xff,0xff) };
    static const Color::Gradation cr(Grad,sizeof(Grad)/sizeof(Grad[0]));

    GradientFilters<float,Crux::Prewitt3> Prewitt3;
    GradientFilters<float,Crux::Prewitt5> Prewitt5;
    GradientFilters<float,Crux::Prewitt7> Prewitt7;

    GradientFilters<float,Crux::Sobel3>   Sobel3;
    GradientFilters<float,Crux::Sobel5>   Sobel5;
    GradientFilters<float,Crux::Sobel7>   Sobel7;

    GradientFilters<float,Crux::Scharr3>  Scharr3;
    GradientFilters<float,Crux::Scharr5>  Scharr5;


    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        Pixmap<float>   pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        IMG.save(img, "img.png", 0);
        GradientMap<float>     gmap(img.w,img.h);
        GradientThinner        thin(img.w,img.h);

        processGrad(par,gmap,Prewitt3,thin,pxf,cr);
        return 0;
        processGrad(par,gmap,Prewitt5,thin,pxf,cr);
        processGrad(par,gmap,Prewitt7,thin,pxf,cr);
        processGrad(par,gmap,Sobel3,thin,pxf,cr);
        processGrad(par,gmap,Sobel5,thin,pxf,cr);
        processGrad(par,gmap,Sobel7,thin,pxf,cr);
        processGrad(par,gmap,Scharr3,thin,pxf,cr);
        processGrad(par,gmap,Scharr5,thin,pxf,cr);



    }




}
Y_UDONE()
