

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/ramp/map8.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"
#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"
#include "y/ink/ops/gradient/map.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"
#include "y/ink/ops/histogram.hpp"

namespace Yttrium
{
    namespace Ink
    {
        class GradientThinner
        {
        public:

            inline explicit GradientThinner(const unit_t W, const unit_t H) :
            intensity(W,H)
            {
            }

            inline virtual ~GradientThinner() noexcept {}

            template <typename T>
            inline void operator()(Slabs                &slabs,
                                   Histogram            &hist,
                                   const GradientMap<T> &gmap)
            {
                assert(intensity.hasSameSizesThan(gmap.intensity));
                slabs.split(intensity);
                slabs.simt(Optimize<T>,*this,gmap);
                hist.collect(slabs);
            }

            template <typename T>
            static void Optimize(Slab                 &slab,
                                 GradientThinner      &self,
                                 const GradientMap<T> &gmap)

            {       
                static const T zero(0.0);
                static const T half(0.5);
                static const T scal(255.0);
                typedef V2D<T> Vertex;
                const T gmax = gmap.nmax;
                Histogram::Word * const H = Histogram::BinsFrom(slab);
                if(gmax<=zero)
                {
                    slab.load( Coerce(self.intensity), zero );
                }
                else
                {
                    assert(gmax>0);
                    for(size_t k=slab.count();k>0;--k)
                    {
                        const HSegment       &seg = slab.hseg[k];
                        const unit_t          y   = seg.y;
                        const PixRow<Vertex> &vec = gmap.direction(y);
                        const PixRow<T>      &nrm = gmap.intensity(y);
                        PixRow<uint8_t>      &opt = Coerce(self.intensity(y));
                        for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
                        {
                            const T      g0  = nrm(x);
                            uint8_t      &out = opt[x]; if(g0<=zero) { out = 0; continue; }
                            const Vertex v   = vec(x);
                            const unit_t dx  = static_cast<unit_t>( floor(v.x+half) ); assert( dx>=-1 && dx <=1 );
                            const unit_t dy  = static_cast<unit_t>( floor(v.y+half) ); assert( dy>=-1 && dy <=1 );

                            if( gmap.intensity[y+dy][x+dx] > g0 ) { out = 0; continue; }
                            if( gmap.intensity[y-dy][x-dx] > g0 ) { out = 0; continue; }
                            out = static_cast<uint8_t>(floor( scal*(g0/gmax) + half ));
                            ++H[out];
                        }
                    }
                }
            }


            const Pixmap<uint8_t> intensity;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientThinner);
        };

    }
}

using namespace Yttrium;
using namespace Ink;

#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/stream/libc/output.hpp"



static inline
void processGrad(Slabs                  &par,
                 GradientMap<float>     &gmap,
                 const Gradient<float>  &grad,
                 GradientThinner        &thin,
                 const Pixmap<float>    &pxf,
                 const Color::Ramp      &cr)
{
    std::cerr << "Apply " << grad.name << std::endl;
    Codec &IMG = Codecs::Location();
    Histogram hist;
    gmap(par,grad,pxf);
    std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;
    thin(par,hist,gmap);
    (std::cerr << "saving..." << std::endl).flush();



    {
        {
            const Color::FlexibleRamp ramp(cr,gmap.nmin,gmap.nmax);
            const String fileName = "grad-" + grad.name + ".png";
            IMG.save(gmap.intensity,fileName, 0, par, ramp);
        }

        {
            const Color::Map8 ramp(cr);
            const String fileName = "thin-" + grad.name + ".png";
            IMG.save(thin.intensity,fileName, 0,par,ramp);
        }

        {
            const String fileName = "hist-" + grad.name + ".dat";
            hist.save(fileName);
        }

    }
}

Y_UTEST(grad)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codecs &               IMG = Ink::Codecs::Std();

    static const RGBA Grad[] = {
        RGBA(0x00,0x00,0x00),
        RGBA(0x00,0x00,0xff),
        RGBA(0x00,0xff,0x00),
        RGBA(0xff,0x00,0x00),
        RGBA(0xff,0xff,0xff) };
    static const Color::Gradation cr(Grad,sizeof(Grad)/sizeof(Grad[0]));

    GradientFilters<float,Crux::Prewitt3> Prewitt3;
    GradientFilters<float,Crux::Prewitt5> Prewitt5;
    GradientFilters<float,Crux::Prewitt7> Prewitt7;

    GradientFilters<float,Crux::Sobel3>   Sobel3;
    GradientFilters<float,Crux::Sobel5>   Sobel5;
    GradientFilters<float,Crux::Sobel7>   Sobel7;

    GradientFilters<float,Crux::Scharr3>  Scharr3;
    GradientFilters<float,Crux::Scharr5>  Scharr5;


    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        Pixmap<float>   pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        IMG.save(img, "img.png", 0);
        GradientMap<float>     gmap(img.w,img.h);
        GradientThinner        thin(img.w,img.h);

        processGrad(par,gmap,Prewitt3,thin,pxf,cr);
        processGrad(par,gmap,Prewitt5,thin,pxf,cr);
        processGrad(par,gmap,Prewitt7,thin,pxf,cr);
        processGrad(par,gmap,Sobel3,thin,pxf,cr);
        processGrad(par,gmap,Sobel5,thin,pxf,cr);
        processGrad(par,gmap,Sobel7,thin,pxf,cr);
        processGrad(par,gmap,Scharr3,thin,pxf,cr);
        processGrad(par,gmap,Scharr5,thin,pxf,cr);



    }




}
Y_UDONE()
