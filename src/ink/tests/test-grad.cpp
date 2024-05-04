

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"
#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"
#include "y/ink/ops/gradient/map.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"

using namespace Yttrium;
using namespace Ink;


static inline
void processGrad(Slabs                 &par,
                 GradientMap<float>    &gmap,
                 const Gradient<float> &grad,
                 const Pixmap<float>   &pxf,
                 const Color::Ramp     &cr)
{
    std::cerr << "Apply " << grad.name << std::endl;
    Codec &IMG = Codecs::Location();
    gmap(par,grad,pxf);
    {
        const Color::FlexibleRamp ramp(cr,gmap.nmin,gmap.nmax);
        const String fileName = "img-" + grad.name + ".png";
        IMG.save(gmap.intensity,fileName, 0, par, ramp);
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
        GradientMap<float> gmap(img.w,img.h);

        processGrad(par,gmap,Prewitt3,pxf,cr);
        processGrad(par,gmap,Prewitt5,pxf,cr);
        processGrad(par,gmap,Prewitt7,pxf,cr);
        processGrad(par,gmap,Sobel3,pxf,cr);
        processGrad(par,gmap,Sobel5,pxf,cr);
        processGrad(par,gmap,Sobel7,pxf,cr);
        processGrad(par,gmap,Scharr3,pxf,cr);
        processGrad(par,gmap,Scharr5,pxf,cr);

#if 0
        g(par,Prewitt5,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-prewitt5.png", 0, par, ramp);
        }

        g(par,Prewitt7,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-prewitt7.png", 0, par, ramp);
        }

        g(par,Sobel3,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-sobel3.png", 0, par, ramp);
        }

        g(par,Sobel5,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-sobel5.png", 0, par, ramp);
        }

        g(par,Sobel7,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-sobel7.png", 0, par, ramp);
        }

        g(par,Scharr3,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-scharr3.png", 0, par, ramp);
        }

        g(par,Scharr5,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.nmin,g.nmax);
            IMG.Codec::save(g.nrm, "img-scharr5.png", 0, par, ramp);
            IMG.Codec::save(g.opt, "opt-scharr5.png", 0, par, ramp);

        }
#endif

    }




}
Y_UDONE()
