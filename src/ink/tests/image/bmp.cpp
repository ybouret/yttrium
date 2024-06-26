
#include "y/ink/image/format/bmp.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"

using namespace Yttrium;



Y_UTEST(bmp)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
  
    Ink::Options        opts;
    Ink::Format::Handle fmt = new Ink::FormatBMP();

    {
        Y_CHECK(fmt->matches("hello.bmp"));
        Y_CHECK(fmt->matches("Hello.BmP"));
    }

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    Ink::IndexToRGBA::Load(img,slabs,ran);

    fmt->save(img, "img.bmp", 0);
    Ink::Codec::Image cpy( fmt->load("img.bmp",0) );


}
Y_UDONE()

