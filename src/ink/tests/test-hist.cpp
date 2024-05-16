#include "y/ink/ops/lut8.hpp"
#include "y/ink/ops/histogram.hpp"
#include "y/ink/image/codecs.hpp"


#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/color/grayscale.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md.hpp"
#include "y/memory/digest.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"
#include <cstring>

#include "y/color/ramp/dark-to-bright.hpp"
#include "y/color/conv.hpp"
#include "y/text/hexadecimal.hpp"

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
    Codec  &      IMG = Ink::Codecs::Std();
    Slabs         par( InParallel );
    Histogram    hist;


    const Color::DarkToBright Gradient;

    if(argc>1)
    {
        (std::cerr << "Loading " << argv[1] << std::endl).flush();
        Pixmap<RGBA> img = IMG.load(argv[1],0);

        std::cerr << "img32 =" << img.crc32() << " @row=" << img.brow << std::endl;
        
        //std::cerr << img << std::endl;
        IMG.save(img, "hist-img.png",0);
        return 0;

        Pixmap<uint8_t> msk(img.w,img.h);
        //std::cerr << img << std::endl;


        (std::cerr << "Building Histogram..." << std::endl).flush();
        {
            const uint64_t pixels = img.n;
            const uint64_t mark = WallTime::Ticks();
            hist.load(par,img, Color::GrayScale::Pack<uint8_t,RGBA>, msk);
            const uint64_t ell  = WallTime::Ticks() - mark;
            const long double rate = (static_cast<long double>(pixels) / tmx(ell))*1e-6L;
            std::cerr << "rate=" << rate << " Mpx/s" << std::endl;
        }

        // hashing
        const Digest    hh = Hashing::MD::Of(hfn,hist);
        std::cerr << "hash=" << hh << std::endl;

        return 0;

        Pixmap<RGBA>    tgt(par, Color::GrayScale::ByteTo<RGBA>, msk);

        (std::cerr << "Saving..." << std::endl).flush();
        IMG.save(img, "hist-img.png",0);
        IMG.save(tgt, "hist-msk.png",0);

        //IMG.renderRamp("ramp.png", Gradient, 800, 100);

        std::cerr << "Converting mask to GreyScale" << std::endl;
        Pixmap<float> pxf(par,ByteToFloat,msk);
        IMG.save(pxf, "hist-flt.png", 0, par, Gradient);
        

        LookUpTable8 lut;
        uint8_t lower=0,upper=0;
        if(hist.find(lower,upper))
        {
            std::cerr << "find: " << int(lower) << " -> " << int(upper) << std::endl;
            lut.stretch(lower,upper);
        }
        lut(par,msk);
        pxf.ld(par, ByteToFloat,msk);
        IMG.Codec::save(pxf, "hist-nrm.png", 0, par, Gradient);

        //hist.save("hist.dat");
        
    }

    if(false)
    {
        uint8_t k=0;
        for(unsigned i=0;i<16;++i)
        {
            for(unsigned j=0;j<16;++j)
            {
                std::cerr << " 0x" << Hexadecimal(k++) << ",";
            }
            std::cerr << std::endl;
        }
    }



}
Y_UDONE()

