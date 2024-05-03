
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

#include "y/color/gradation.hpp"
#include "y/color/conv.hpp"

using namespace Yttrium;
using namespace Ink;

static inline float ByteToFloat(const uint8_t u) noexcept
{
    return Color::Conv<float>::Unit[u];
}

Y_UTEST(hist)
{
    Hashing::SHA1 hfn;
    WallTime      tmx;
    Codecs &      IMG = Ink::Codecs::Std();

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Histogram<size_t>      hist;

    static const RGBA Grad[] = { 
        RGBA(0x00,0x00,0x00),
        RGBA(0x00,0x00,0xff),
        RGBA(0x00,0xff,0x00),
        RGBA(0xff,0x00,0x00) };
    static const Color::Gradation Gradient(Grad,sizeof(Grad)/sizeof(Grad[0]));

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
        std::cerr << "lower=" << int(hist.lower) << std::endl;
        std::cerr << "upper=" << int(hist.upper) << std::endl;

        Pixmap<RGBA>    tgt(par, Color::GrayScale::ByteTo<RGBA>, msk);

        (std::cerr << "Saving..." << std::endl).flush();
        IMG.save(img, "hist-img.png",0);
        IMG.save(tgt, "hist-msk.png",0);

        IMG.renderRamp("ramp.png", Gradient, 800, 200);

        Pixmap<float> pxf(par,ByteToFloat,msk);
        IMG.Codec::save(pxf, "hist-flt.png", 0, par, Gradient);
        hist.save("hist.dat");
        
    }




}
Y_UDONE()

