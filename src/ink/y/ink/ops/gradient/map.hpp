
//! \file

#ifndef Y_Ink_Gradient_Map_Included
#define Y_Ink_Gradient_Map_Included 1

#include "y/ink/ops/gradient.hpp"

namespace Yttrium
{

    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Map of gradient direction + norm
        //
        //
        //______________________________________________________________________

        template <typename T>
        class GradientMap
        {
        public:
            typedef V2D<T> Vertex;

            inline explicit GradientMap(const unit_t W, const unit_t H) :
            direction(W,H),
            intensity(W,H),
            nmin(0),
            nmax(0)
            {
            }

            inline virtual ~GradientMap() noexcept {}


            const Pixmap<Vertex> direction; //!< normalized direction
            const Pixmap<T>      intensity; //!< intensity
            const T              nmin;
            const T              nmax;

            template <typename U> inline
            T apply(const Pixmap<U>   &source,
                    const Coord        origin,
                    const Gradient<T> &grad)
            {
                Vertex &vtx = Coerce(direction(origin));
                const T gx  = grad.dx().apply(vtx.x,source,origin);
                const T gy  = grad.dy().apply(vtx.y,source,origin);
                const T gn  = (Coerce(intensity(origin)) = MKL::Hypotenuse(gx,gy));
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
            void operator()(Slabs &slabs, const Gradient<T> &grad, const Pixmap<U> &source)
            {
                assert(source.hasSameSizesThan(direction));
                assert(source.hasSameSizesThan(intensity));
                slabs.split(direction);
                slabs.simt(Apply<U>,*this,grad,source);
                slabs.getMinMax(Coerce(nmin), Coerce(nmax));
                //slabs.simt(Optimize,*this);
            }





        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientMap);

            template <typename U> static inline
            void Apply(Slab &slab, GradientMap &self, const Gradient<T> &grad, const Pixmap<U> &source)
            {
                for(size_t k=slab.count();k>0;--k)
                {
                    const HSegment &seg = slab.hseg[k];
                    Coord           pos = seg.start();
                    for(unit_t i=seg.w;i>0;--i,++pos.x)
                    {
                        self.apply(source,pos,grad);
                    }
                }
                slab.scanMinMax(self.intensity);
            }
        };



    }

}

#if 0
static void Optimize(Slab &slab, GradMap &g)
{
    const T zero(0);
    const T half(0.5);
    for(size_t k=slab.count();k>0;--k)
    {
        const HSegment       &seg = slab.hseg[k];
        const unit_t          y   = seg.y;
        const PixRow<Vertex> &vec = g.vec(y);
        const PixRow<T>      &nrm = g.nrm(y);
        PixRow<T>            &opt = Coerce(g.opt(y));
        for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
        {
            const T      g0  = nrm(x);
            T           &out = opt[x];
            if(g0<=zero)
            {
                out = zero; continue;
            }
            else
            {
                const Vertex v  = vec(x);
                const unit_t dx = static_cast<unit_t>( floor(v.x+half) ); assert( dx>=-1 && dx <=1 );
                const unit_t dy = static_cast<unit_t>( floor(v.y+half) ); assert( dy>=-1 && dy <=1 );

                if( g.nrm[y+dy][x+dx] > g0 )
                {
                    out = zero; continue;
                }

                if( g.nrm[y-dy][x-dx] > g0 )
                {
                    out = zero; continue;
                }

                out = g0;
            }
        }
    }
}
#endif

#endif
