
#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/random/park-miller.hpp"
#include "y/ink/parallel/index-to-rgba.hpp"
#include "y/sequence/vector.hpp"

using namespace Yttrium;



Y_UTEST(image)
{

    Ink::Codecs & IMG = Ink::Codecs::Std();
    {
        Vector<Ink::Format::Handle> formats;
        IMG.fetch(formats);
        std::cerr << IMG.callSign() << " : " << formats << std::endl;
    }

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Ink::Slabs             slabs( crew );
    Ink::Codec::Image      img(200,100);
    Random::ParkMiller     ran;
    Ink::IndexToRGBA::Load(img,slabs,ran);

    IMG.save(img, "image.bmp", 0);
    IMG.save(img, "image.jpg", 0);
    IMG.save(img, "image.png", 0);
    IMG.save(img, "image.tif", 0);




}
Y_UDONE()

