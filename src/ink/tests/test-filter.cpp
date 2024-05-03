
#include "y/ink/ops/filter.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"

using namespace Yttrium;
using namespace Ink;

static const int w23[3][2] =
{
    {-1,1},
    {-1,1},
    {-1,1}
};



Y_UTEST(filter)
{
    Filter<float> F("w23",&w23[0][0],Area( Coord(0,1), 2, 3));

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codecs &               IMG = Ink::Codecs::Std();

    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        IMG.save(img, "img.png", 0);
        Pixmap<uint8_t>  pxu(par,Color::GrayScale::Pack<uint8_t,RGBA>,img);
        Pixmap<float>    pxf(img.w,img.h);
        float fmin=0, fmax=0;
        F(par,pxf,fmin,fmax,pxu);
        std::cerr << "fmin=" << fmin << ", fmax=" << fmax << std::endl;
    }


}
Y_UDONE()
