//! \file

#ifndef Y_Ink_Gradient_Thinner_Included
#define Y_Ink_Gradient_Thinner_Included 1

#include "y/ink/ops/gradient/map.hpp"
#include "y/ink/ops/histogram.hpp"
#include "y/ink/ops/gradient/pixel-force.hpp"

namespace Yttrium
{

    namespace Ink
    {

       

        //______________________________________________________________________
        //
        //
        //
        //! build pixel power from gradient map
        //
        //
        //______________________________________________________________________
        class GradientThinner : public Pixmap<uint8_t>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit GradientThinner(const unit_t W, const unit_t H); //!< setup
            virtual ~GradientThinner() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build pixel power
            /**
             - find gradient maxima
             - build gradient maxima histogram
             - compute dual threshold
             - apply to intensity
             */
            template <typename T>
            inline void operator()(Slabs                &slabs,
                                   Histogram            &hist,
                                   const GradientMap<T> &gmap)
            {
                assert(hasSameSizesThan(gmap.intensity));
                //slabs.split(force);
                slabs(Optimize<T>,*this,gmap);
                finalize(slabs,hist);
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(GradientThinner);
            void finalize(Slabs &slabs, Histogram &hist);

            //! optimize gradient map
            /**
             - keep only maxima of gradient
             - rescale to byte w.r.t gmax
             - update local histogram
             */
            template <typename T>static
            void Optimize(Slab                 &slab,
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
                    slab.load(self,zero);
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
                        RowType              &opt = self[y];
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

        };

    }

}
#endif

