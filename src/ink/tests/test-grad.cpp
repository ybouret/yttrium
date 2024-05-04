

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"
#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"
#include "y/ink/ops/gradient/map.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"

namespace Yttrium
{
    namespace Ink
    {
        template <typename T>
        class GradientThinner
        {
        public:
            typedef V2D<T> Vertex;

            inline explicit GradientThinner(const unit_t W, const unit_t H) :
            intensity(W,H),
            nmin(0),
            nmax(0)
            {
            }

            inline virtual ~GradientThinner() noexcept {}

            inline void operator()(Slabs                &slabs,
                                   const GradientMap<T> &gmap)
            {
                assert(intensity.hasSameSizesThan(gmap.intensity));
                slabs.split(intensity);
                slabs.simt(Optimize,*this,gmap);
                Coerce(nmax) = gmap.nmax;
                {
                    T &gmin = ( Coerce(nmin) = nmax );
                    for(size_t i= slabs.simt.size();i>0;--i)
                    {
                        const Slab &slab = slabs.simt[i]; if(slab.count()<=0) continue;
                        gmin = Min(gmin, *slab.as<T>(1) );
                    }
                }
            }

            static void Optimize(Slab                 &slab,
                                 GradientThinner      &self,
                                 const GradientMap<T> &gmap)

            {
                const T zero(0);
                const T half(0.5);
                T     & gmin = (*slab.as<T>(1)=gmap.nmax);
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment       &seg = slab.hseg[k];
                    const unit_t          y   = seg.y;
                    const PixRow<Vertex> &vec = gmap.direction(y);
                    const PixRow<T>      &nrm = gmap.intensity(y);
                    PixRow<T>            &opt = Coerce(self.intensity(y));
                    for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
                    {
                        const T      g0  = nrm(x);
                        T           &out = opt[x]; if(g0<=zero) { out = zero; continue; }
                        const Vertex v   = vec(x);
                        const unit_t dx  = static_cast<unit_t>( floor(v.x+half) ); assert( dx>=-1 && dx <=1 );
                        const unit_t dy  = static_cast<unit_t>( floor(v.y+half) ); assert( dy>=-1 && dy <=1 );

                        if( gmap.intensity[y+dy][x+dx] > g0 ) { out = zero; continue; }
                        if( gmap.intensity[y-dy][x-dx] > g0 ) { out = zero; continue; }
                        out = g0;
                        if(g0<gmin) gmin = g0;
                    }
                }
            }


            const Pixmap<T> intensity;
            const T         nmin;
            const T         nmax;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientThinner);
        };

    }
}

using namespace Yttrium;
using namespace Ink;


static inline
void processGrad(Slabs                  &par,
                 GradientMap<float>     &gmap,
                 const Gradient<float>  &grad,
                 GradientThinner<float> &thin,
                 const Pixmap<float>    &pxf,
                 const Color::Ramp      &cr)
{
    std::cerr << "Apply " << grad.name << std::endl;
    Codec &IMG = Codecs::Location();
    gmap(par,grad,pxf);
    std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;
    thin(par,gmap);
    std::cerr << "thin: " << thin.nmin << " -> " << thin.nmax << std::endl;
    (std::cerr << "saving..." << std::endl).flush();
    {
        {
            const Color::FlexibleRamp ramp(cr,gmap.nmin,gmap.nmax);
            const String fileName = "grad-" + grad.name + ".png";
            IMG.save(gmap.intensity,fileName, 0, par, ramp);
        }

        {
            const Color::FlexibleRamp ramp(cr,thin.nmin,thin.nmax);
            const String fileName = "thin-" + grad.name + ".png";
            IMG.save(thin.intensity,fileName, 0, par, ramp);
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
        GradientThinner<float> thin(img.w,img.h);

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
