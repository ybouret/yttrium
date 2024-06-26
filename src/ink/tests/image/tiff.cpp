

#include "y/ink/image/format/tiff.hpp"
#include "y/utest/run.hpp"
#include "y/text/ops.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"

using namespace Yttrium;



Y_UTEST(tiff)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
    Ink::Options           opts;

#if 1
    Ink::Format::Handle    fmt = new Ink::FormatTIFF();

    {
        Y_CHECK(fmt->matches("hello.tif"));
        Y_CHECK(fmt->matches("Hello.TIFf"));
    }

    Ink::Codec::Image  img(200,100);
    Random::ParkMiller ran;
    Ink::IndexToRGBA::Load(img,slabs,ran);

    fmt->save(img, "img.tif", 0);
    Ink::Codec::Image cpy = fmt->load("img.tif",0);
#endif

}
Y_UDONE()

