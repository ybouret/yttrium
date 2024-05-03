#include "y/ink/ops/blur.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"

using namespace Yttrium;
using namespace Ink;

Y_UTEST(blur)
{
    float sig = 2.4;
    if(argc>1)
    {
        sig = ASCII::Convert::ToReal<float>(argv[1],"sigma");
    }

    Blur<long double> blur(sig);

    if(argc>2)
    {
        Concurrent::Topology   topo;
        Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
        Slabs                  par( crew );

        Codecs &        IMG = Ink::Codecs::Std();
        Pixmap<RGBA>    img = IMG.load(argv[2],0);
        Pixmap<RGBA>    blr(img.w,img.h);
        Pixmap<uint8_t> img8(par,Color::GrayScale::Pack<uint8_t,RGBA>,img);
        Pixmap<uint8_t> blr8(img.w,img.h);

#if 0
        for(unit_t y=0;y<img.h;++y)
        {
            for(unit_t x=0;x<img.w;++x)
            {
                blur.apply(blr[y][x], img,  Coord(x,y)  );
                blur.apply(blr8[y][x],img8, Coord(x,y) );
            }
        }
#endif

        blur(par,blr,img);
        blur(par,blr8,img8);

        IMG.save(img,"blur-in.png",0);
        IMG.save(blr,"blur-out.png",0);

        IMG.Codec::save(img8, "blur8-in.png",  0, par, Color::GrayScale::ByteTo<RGBA>);
        IMG.Codec::save(blr8, "blur8-out.png", 0, par, Color::GrayScale::ByteTo<RGBA>);

    }

    Y_SIZEOF(Blur<float>);
    Y_SIZEOF(Blur<double>);
    Y_SIZEOF(Blur<long double>);





}
Y_UDONE()

