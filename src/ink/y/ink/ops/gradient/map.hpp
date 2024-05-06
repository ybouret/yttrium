
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
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef V2D<T> Vertex; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            inline explicit GradientMap(const unit_t W, const unit_t H) :
            direction(W,H),
            intensity(W,H),
            nmin(0),
            nmax(0)
            {
            }

            //! cleanup
            inline virtual ~GradientMap() noexcept {}


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compute gradient and retrieve min/max
            template <typename U> inline
            void operator()(Slabs &slabs, const Gradient<T> &grad, const Pixmap<U> &source)
            {
                assert(source.hasSameSizesThan(direction));
                assert(source.hasSameSizesThan(intensity));
                slabs.split(direction);
                slabs.simt(Apply<U>,*this,grad,source);
                slabs.getMinMax(Coerce(nmin), Coerce(nmax));
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Pixmap<Vertex> direction; //!< normalized direction
            const Pixmap<T>      intensity; //!< intensity
            const T              nmin;      //!< minimal norm
            const T              nmax;      //!< maximal norm


        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientMap);

            //! compute gradient at source[origin]
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

            //! parallel apply
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



#endif
