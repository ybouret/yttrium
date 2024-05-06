#include "y/ink/ops/gradient/thinner.hpp"

namespace Yttrium
{

    namespace Ink
    {
        GradientThinner:: ~GradientThinner() noexcept {}
        
        GradientThinner:: GradientThinner(const unit_t W, const unit_t H) : 
        intensity(W,H),
        label(W,H)
        {
        }

        static inline
        void Separate(Slab            &slab,
                      Pixmap<uint8_t> &grad,
                      const uint8_t    feeble,
                      const uint8_t    strong)
        {
            for(size_t k=slab.count();k>0;--k)
            {
                const HSegment        seg = slab.hseg[k];
                PixRow<uint8_t>      &opt = grad[seg.y];
                for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
                {
                    uint8_t &pix = opt[x];
                    if(pix<feeble)  { pix=Vanish; continue; }
                    if(pix>=strong) { pix=Strong; continue; }
                    pix = Feeble;
                }
            }
        }
        
        void GradientThinner:: finalize(Slabs &slabs, Histogram &hist)
        {
            hist.ldz();
            hist.collect(slabs);
            const uint8_t strong = hist.threshold();
            const uint8_t feeble = strong >> 1;
            slabs(Separate,Coerce(intensity),feeble,strong);
        }

    }

}

