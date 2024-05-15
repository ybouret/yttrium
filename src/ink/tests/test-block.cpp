#include "y/ink/ops/block/ops.hpp"
#include "y/ink/ops/block.hpp"

#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/concurrent/loop/mono.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/channels.hpp"
#include "y/color/grayscale.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/rgb/x11.hpp"
#include "y/color/conv.hpp"



using namespace Yttrium;
using namespace Ink;

#include <typeinfo>

template <
typename T,
typename BLOCK,
typename PIXEL2RGBA> static inline
void Process(Pixmap<T>       &target,
             const Pixmap<T> &source,
             Slabs           &slabs,
             const BLOCK     &block,
             PIXEL2RGBA      &p2c)
{
    static const size_t N   = BLOCK::N;
    static const Codec &IMG = Codecs::Instance();

    String pfx;
    if( typeid(T) == typeid(float)   ) { pfx = "flt"; }
    if( typeid(T) == typeid(uint8_t) ) { pfx = "u8";  }
    if( typeid(T) == typeid(RGBA)    ) { pfx = "rgba";  }

    if( "" == pfx ) throw Exception("couldn't use given type");


    pfx += Formatted::Get("-%dx%d", int(BLOCK::W), int(BLOCK::H));

    std::cerr << "Processing " << pfx << std::endl;


    // saving original image
    {
        std::cerr << "\tsave original" << std::endl;
        const String fileName = pfx + "-img.png";
        IMG.save(source, fileName, 0, slabs, p2c);
    }

    // min
    block(slabs,target,BlockOps<T>::template Minimum<N>,source);
    {
        std::cerr << "\tsave min" << std::endl;
        const String fileName = pfx + "-min.png";
        IMG.save(target, fileName, 0, slabs, p2c);
    }

    // max
    block(slabs,target,BlockOps<T>::template Maximum<N>,source);
    {
        std::cerr << "\tsave max" << std::endl;
        const String fileName = pfx + "-max.png";
        IMG.save(target, fileName, 0, slabs, p2c);
    }

    // average
    block(slabs,target,BlockOps<T>::template Average<N>,source);
    {
        std::cerr << "\tsave average" << std::endl;
        const String fileName = pfx + "-ave.png";
        IMG.save(target, fileName, 0, slabs, p2c);
    }

    // median
    block(slabs,target,BlockOps<T>::template Median<N>,source);
    {
        std::cerr << "\tsave median" << std::endl;
        const String fileName = pfx + "-med.png";
        IMG.save(target, fileName, 0, slabs, p2c);
    }
}


template <
typename T,
typename PIXEL2RGBA> static inline
void Process(Pixmap<T>       &target,
             const Pixmap<T> &source,
             Slabs           &slabs,
             PIXEL2RGBA      &p2c)
{
    { Block<1,1> blk3; Process(target, source, slabs, blk3, p2c); }
    { Block<2,2> blk5; Process(target, source, slabs, blk5, p2c); }
}

static inline RGBA RGBA_To_RGBA(const RGBA   &c) { return c; }
static inline RGBA Byte_To_RGBA(const uint8_t u) { return RGBA(u,u,u); }
static inline RGBA Float_To_RGBA(const float &f)
{
    return Byte_To_RGBA( Color::Conv<float>::ToByte(f) );
}





Y_UTEST(block)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    const Color::RampColor bw[2] = { Y_Black, Y_White };
    const Color::Gradation cr(bw,2);

    Codec &IMG = Codecs::Std();


    if(argc>1)
    {
        const Pixmap<RGBA>  img = IMG.load(argv[1],0);

        {
            Pixmap<RGBA>        target(img.w,img.h);
            Process(target,img,par,RGBA_To_RGBA);

        }

        {
            const Pixmap<float>  source(par,Color::GrayScale::Pack<float,RGBA>,img);
            Pixmap<float>        target(img.w,img.h);
            Process(target,source,par,Float_To_RGBA);
        }

        {
            const Pixmap<uint8_t>  source(par,Color::GrayScale::Pack<uint8_t,RGBA>,img);
            Pixmap<uint8_t>        target(img.w,img.h);
            Process(target,source,par,Byte_To_RGBA);
        }




#if 0
        const Pixmap<RGBA>  img = IMG.load(argv[1],0);
        const Pixmap<float> pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        
        IMG.save(img, "img-rgb.png", 0);
        Pixmap<RGBA>        tgt(img.w,img.h);
        Pixmap<float>       out(img.w,img.h);
        IMG.save(pxf, "img-flt.png", 0, par, cr);

#endif



#if 0
        blk(par,out,BlockOps<float>::Average<Blk3x3::N>,pxf);
        IMG.save(out, "ave-flt.png", 0, par, cr);

        blk(par,tgt,BlockOps<RGBA>::Average<Blk3x3::N>,img);
        IMG.save(tgt, "ave-rgb.png", 0);


        blk(par,out,BlockOps<float>::Minimum<Blk3x3::N>,pxf);
        IMG.save(out, "min-flt.png", 0, par, cr);

        blk(par,tgt,BlockOps<RGBA>::Minimum<Blk3x3::N>,img);
        IMG.save(tgt, "min-rgb.png", 0);


        blk(par,out,BlockOps<float>::Maximum<Blk3x3::N>,pxf);
        IMG.save(out, "max-flt.png", 0, par, cr);

        blk(par,tgt,BlockOps<RGBA>::Maximum<Blk3x3::N>,img);
        IMG.save(tgt, "max-rgb.png", 0);

        blk(par,out,BlockOps<float>::Median<Blk3x3::N>,pxf);
        IMG.save(out, "med-flt.png", 0, par, cr);

        blk(par,tgt,BlockOps<RGBA>::Median<Blk3x3::N>,img);
        IMG.save(tgt, "med-rgb.png", 0);
#endif
    }


}
Y_UDONE()
