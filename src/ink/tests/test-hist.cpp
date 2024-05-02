
#include "y/ink/ops/histogram.hpp"
#include "y/ink/image/codecs.hpp"

#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/sequence/vector.hpp"
#include "y/color/grayscale.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include <cstring>



using namespace Yttrium;
using namespace Ink;


Y_UTEST(hist)
{
    Hashing::SHA1 hfn;
    WallTime      tmx;
    Codecs &      IMG = Ink::Codecs::Std();

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Histogram<size_t>      hist;

    if(argc>1)
    {
        (std::cerr << "Loading " << argv[1] << std::endl).flush();
        Pixmap<RGBA>    img = IMG.Codec::load(argv[1],0);

        Pixmap<uint8_t> msk(img.w,img.h);
        (std::cerr << "Building Histogram..." << std::endl).flush();
        {
            const uint64_t pixels = img.n;
            const uint64_t mark = WallTime::Ticks();
            hist.make(par,img, Color::GrayScale::From<RGBA>, msk);
            const uint64_t ell  = WallTime::Ticks() - mark;
            const long double rate = (static_cast<long double>(pixels) / tmx(ell))*1e-6L;
            std::cerr << "rate=" << rate << " Mpx/s" << std::endl;
        }

        const Digest    hh = Hashing::MD::Of(hfn,hist);
        std::cerr << "hash=" << hh << std::endl;
        Pixmap<RGBA>    tgt(par, Color::GrayScale::ByteTo<RGBA>, msk);

        (std::cerr << "Saving..." << std::endl).flush();
        IMG.save(img, "hist-img.png",0);
        IMG.save(tgt, "hist-msk.png",0);

    }




}
Y_UDONE()

