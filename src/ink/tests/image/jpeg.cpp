

#include "y/ink/image/format/jpeg.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"

using namespace Yttrium;


Y_UTEST(jpeg)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
    Ink::Options           opts;
    Ink::Format::Handle    fmt = new Ink::FormatJPEG();

    {
        Y_CHECK(fmt->matches("hello.jpg"));
        Y_CHECK(fmt->matches("Hello.jPEg"));
    }

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    Ink::IndexToRGBA::Load(img,slabs,ran);

    opts << "quality=70";
    fmt->save(img, "img.jpg", &opts);

    Ink::Codec::Image cpy = fmt->load("img.jpg",0);




}
Y_UDONE()

