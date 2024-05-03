

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
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
        class GradMap
        {
        public:
            typedef V2D<T>                     Vertex;
            typedef typename Filter<T>::Handle FilterHandle;

            explicit GradMap(const unit_t W, const unit_t H,
                             const FilterHandle &DX, const FilterHandle &DY) :
            fdx(DX), fdy(DY),
            vec(W,H),
            nrm(W,H),
            gmin(0),
            gmax(0)
            {
            }

            const FilterHandle   fdx;
            const FilterHandle   fdy;
            const Pixmap<Vertex> vec;
            const Pixmap<T>      nrm;
            const T              gmin;
            const T              gmax;

            template <typename U> inline
            T apply(const Pixmap<U> &source,
                    const Coord      origin)
            {
                Vertex &vtx = Coerce(vec(origin));
                const T gx  = fdx->apply(vtx.x,source,origin);
                const T gy  = fdy->apply(vtx.y,source,origin);
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
            void operator()(Slabs &slabs, const Pixmap<U> &source)
            {
                assert(source.hasSameSizesThan(vec));
                assert(source.hasSameSizesThan(nrm));
                slabs.split(vec);
                slabs.simt(Apply<U>,*this,source);
                slabs.getMinMax(Coerce(gmin), Coerce(gmax));
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradMap);
            template <typename U> static inline
            void Apply(Slab &slab, GradMap &g, const Pixmap<U> &source)
            {
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment &seg = slab.hseg[k];
                    Coord           pos = seg.start();
                    for(size_t i=seg.w;i>0;--i,++pos.x)
                        g.apply(source,pos);
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

    Filter<float>::Handle DX = new SquareFilter<float,Prewitt3X>();
    Filter<float>::Handle DY = new SquareFilter<float,Prewitt3Y>();


    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        Pixmap<float>   pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        IMG.save(img, "img.png", 0);
        GradMap<float> g(img.w,img.h,DX,DY);
        g(par,pxf);
        std::cerr << "gmin=" << g.gmin << ", gmax=" << g.gmax << std::endl;
     
        {
            const Color::FlexibleRamp ramp(ColorGradient,g.gmin,g.gmax);
            IMG.Codec::save(g.nrm, "img-g.png", 0, par, ramp);
        }

    }



}
Y_UDONE()
