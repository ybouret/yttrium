

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"

using namespace Yttrium;
using namespace Ink;

namespace Yttrium
{
    namespace Ink
    {

        template <typename T>
        class GradDrvs
        {
        public:
            typedef typename Filter<T>::Handle FilterHandle;
            typedef V2D<T>                     Vertex;

            inline explicit GradDrvs(const FilterHandle &DX,
                                     const FilterHandle &DY) noexcept :
            fdx(DX), fdy(DY)
            {
            }



            inline virtual ~GradDrvs() noexcept {}
            const FilterHandle   fdx;
            const FilterHandle   fdy;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradDrvs);
        };

        template <typename T>
        class GradMap
        {
        public:
            typedef V2D<T>                     Vertex;
            typedef typename Filter<T>::Handle FilterHandle;

            explicit GradMap(const unit_t W, const unit_t H) :
            vec(W,H),
            nrm(W,H),
            gmin(0),
            gmax(0)
            {
            }

            const Pixmap<Vertex> vec;
            const Pixmap<T>      nrm;
            const T              gmin;
            const T              gmax;

            template <typename U> inline
            T apply(const Pixmap<U>   &source,
                    const Coord        origin,
                    const GradDrvs<T> &drvs)
            {
                Vertex &vtx = Coerce(vec(origin));
                const T gx  = drvs.fdx->apply(vtx.x,source,origin);
                const T gy  = drvs.fdy->apply(vtx.y,source,origin);
                const T gn  = (Coerce(nrm(origin)) = MKL::Hypotenuse(gx,gy));
                if(gn>0)
                {
                    vtx /= gn;
                    return gn;
                }
                else
                {
                    vtx.x = vtx.y = 0;
                    return 0;
                }
            }

            template <typename U> inline
            void operator()(Slabs &slabs, const GradDrvs<T> &drvs, const Pixmap<U> &source)
            {
                assert(source.hasSameSizesThan(vec));
                assert(source.hasSameSizesThan(nrm));
                slabs.split(vec);
                slabs.simt(Apply<U>,*this,drvs,source);
                slabs.getMinMax(Coerce(gmin), Coerce(gmax));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradMap);
            template <typename U> static inline
            void Apply(Slab &slab, GradMap &g, const GradDrvs<T> &drvs, const Pixmap<U> &source)
            {
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment &seg = slab.hseg[k];
                    Coord           pos = seg.start();
                    for(size_t i=seg.w;i>0;--i,++pos.x)
                        g.apply(source,pos,drvs);
                }
                slab.scanMinMax(g.nrm);
            }

        };


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
    static const Color::Gradation ColorGradient(Grad,sizeof(Grad)/sizeof(Grad[0]));

    Filter<float>::Handle Prewitt3DX = new SquareFilter<float,Prewitt3X>();
    Filter<float>::Handle Prewitt3DY = new SquareFilter<float,Prewitt3Y>();
    GradDrvs<float>       Prewitt3(Prewitt3DX,Prewitt3DY);

    Filter<float>::Handle Prewitt5DX = new SquareFilter<float,Prewitt5X>();
    Filter<float>::Handle Prewitt5DY = new SquareFilter<float,Prewitt5Y>();
    GradDrvs<float>       Prewitt5(Prewitt5DX,Prewitt5DY);

    Filter<float>::Handle Prewitt7DX = new SquareFilter<float,Prewitt7X>();
    Filter<float>::Handle Prewitt7DY = new SquareFilter<float,Prewitt7Y>();
    GradDrvs<float>       Prewitt7(Prewitt7DX,Prewitt7DY);

    Filter<float>::Handle Sobel3DX = new SquareFilter<float,Sobel3X>();
    Filter<float>::Handle Sobel3DY = new SquareFilter<float,Sobel3Y>();
    GradDrvs<float>       Sobel3(Sobel3DX,Sobel3DY);

    Filter<float>::Handle Sobel5DX = new SquareFilter<float,Sobel5X>();
    Filter<float>::Handle Sobel5DY = new SquareFilter<float,Sobel5Y>();
    GradDrvs<float>       Sobel5(Sobel5DX,Sobel5DY);

    Filter<float>::Handle Sobel7DX = new SquareFilter<float,Sobel7X>();
    Filter<float>::Handle Sobel7DY = new SquareFilter<float,Sobel7Y>();
    GradDrvs<float>       Sobel7(Sobel7DX,Sobel7DY);


    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        Pixmap<float>   pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        IMG.save(img, "img.png", 0);
        GradMap<float> g(img.w,img.h);
      
        g(par,Prewitt3,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-prewitt3.png", 0, par, ramp);
        }
      
        g(par,Prewitt5,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-prewitt5.png", 0, par, ramp);
        }

        g(par,Prewitt7,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-prewitt7.png", 0, par, ramp);
        }

        g(par,Sobel3,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-sobel3.png", 0, par, ramp);
        }

        g(par,Sobel5,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-sobel5.png", 0, par, ramp);
        }

        g(par,Sobel7,pxf);
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-sobel7.png", 0, par, ramp);
        }


    }



}
Y_UDONE()
