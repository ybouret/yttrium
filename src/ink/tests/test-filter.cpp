
#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/rgb/x11.hpp"

using namespace Yttrium;
using namespace Ink;


Y_UTEST(filter)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codecs &               IMG = Ink::Codecs::Std();

    static const RGBA Grad[] = {
        Y_Blue,
        Y_Black,
        Y_Red };
    static const Color::Gradation Gradient(Grad,sizeof(Grad)/sizeof(Grad[0]));

    SquareFilter<float,Crux::Prewitt3X> F3X;
    SquareFilter<float,Crux::Prewitt3Y> F3Y;

    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        IMG.save(img, "img.png", 0);
        Pixmap<uint8_t>  pxu(par,Color::GrayScale::Pack<uint8_t,RGBA>,img);
        Pixmap<float>    pxf(img.w,img.h);
        float fmin=0, fmax=0;
        F3X(par,pxf,fmin,fmax,pxu);
        std::cerr << "fmin=" << fmin << ", fmax=" << fmax << std::endl;
        
        {
            const Color::FlexibleRamp ramp(Gradient,fmin,fmax);
            IMG.Codec::save(pxf, "img-x.png", 0, par, ramp);
        }
        F3Y(par,pxf,fmin,fmax,pxu);
        std::cerr << "fmin=" << fmin << ", fmax=" << fmax << std::endl;
        {
            const Color::FlexibleRamp ramp(Gradient,fmin,fmax);
            IMG.Codec::save(pxf, "img-y.png", 0, par, ramp);
        }
    }



}
Y_UDONE()
