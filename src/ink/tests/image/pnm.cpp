

#include "y/ink/image/format/pnm.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"

using namespace Yttrium;



Y_UTEST(pnm)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );

    Ink::FormatOptions     opts;
    Ink::Format::Handle    fmt = new Ink::FormatPNM();

    {
        Y_CHECK(fmt->matches("hello.ppm"));
        Y_CHECK(fmt->matches("Hello.PBm"));
    }

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    Ink::IndexToRGBA::Load(img,slabs,ran);
    for(int i=1;i<argc;++i) opts << argv[i];

    fmt->save(img, "img.ppm", &opts);
    //Ink::Codec::Image cpy = fmt->load("img.bmp",0);


}
Y_UDONE()

