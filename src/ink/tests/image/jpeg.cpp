

#include "y/ink/image/format/jpeg.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"

using namespace Yttrium;

namespace Yttrium
{
    namespace Ink
    {



    }

    static inline
    void LoadIndx(Ink::Slab          &slab,
                  Ink::Codec::Image &target,
                  Random::Bits      &ran)
    {

        Ink::RGBA color;
        {
            Y_LOCK(slab.sync);
            color.r = ran.in<uint8_t>(100,255);
            color.g = ran.in<uint8_t>(100,255);
            color.b = ran.in<uint8_t>(100,255);
            (std::cerr << "ran @" << (void*)&ran <<", color[" << slab.indx << "]=" << color << std::endl).flush();
        }
        for(size_t k=slab.count();k>0;--k)
        {
            const Ink::HSegment    s = slab.hseg[k];
            Ink::Codec::ImageRow &r = target[s.y];
            for(unit_t i=s.w,x=s.x;i>0;--i,++x)
            {
                r[x] = color;
            }
        }
    }

}

Y_UTEST(format_jpeg)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
    Ink::FormatOptions     opts;
    Ink::Format::Handle    fmt = new Ink::FormatJPEG();

    {
        Y_CHECK(fmt->matches("hello.jpg"));
        Y_CHECK(fmt->matches("Hello.jPEg"));
    }

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    slabs(LoadIndx,img,ran);

    fmt->save(img, "img.jpg", 0);

    //Ink::Codec::Image cpy = fmtBMP->load("img.bmp",0);




}
Y_UDONE()

