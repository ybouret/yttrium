#include "y/ink/ops/histogram.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Ink
    {

        Histogram:: ~Histogram() noexcept {}
        
        Histogram:: Histogram() noexcept :total(0),  data()
        {
            ldz();
        }

        void Histogram:: ldz() noexcept {
            memset(data,0,sizeof(data));
            Coerce(total) = 0;
        }

        Histogram::Word Histogram:: operator[](const uint8_t i) const noexcept
        {
            return data[i];
        }

        void Histogram:: add(const Word * const h) noexcept
        {
            assert(0!=h);
            for(unsigned i=0;i<Bins;++i)
            {
                const size_t delta = h[i];
                data[i] += delta;
                Coerce(total) += delta;
            }
        }

        bool Histogram:: find(uint8_t &lower, uint8_t &upper) const noexcept
        {
            lower = 0;
            upper = Bins-1;
            bool ok = false;
            for(unsigned i=0;i<256;++i)
            {
                if(0!=data[i])
                {
                    lower = static_cast<uint8_t>(i);
                    ok    = true;
                    break;
                }
            }

            if(ok)
            {
                for(int i=255;i>=0;--i)
                {
                    if(0!=data[i])
                    {
                        upper = static_cast<uint8_t>(i);
                        break;
                    }
                }
                assert(upper>=lower);
            }
            
            return ok;
        }

        size_t      Histogram:: measure() const noexcept { return sizeof(data); }
        const void *Histogram:: ro_addr() const noexcept { return &data[0];     }

        void Histogram:: write(OutputStream &fp) const
        {
            for(unsigned j=0;j<Bins;++j)
            {
                fp("%u %g\n",j,double(data[j]));
            }
        }


        void Histogram:: collect(const Slabs &slabs)
        {
            for(size_t i=1;i<=slabs.size();++i)
            {
                const Slab  &slab = slabs[i];
                if(slab.count()<=0) continue;
                add( slab.as<Word>(Bins) );
            }
        }

        Histogram:: Word * Histogram:: BinsFrom(Slab &slab)
        {
            return slab.ldz().as<Word>(Bins);
        }

        void Histogram:: load(Slabs &slabs, const Pixmap<uint8_t> &mask)
        {
            ldz();
            slabs.split(mask);
            slabs.simt(LoadDirectly,mask);
            collect(slabs);
        }

        void Histogram:: LoadDirectly(Slab                  &slab,
                                      const Pixmap<uint8_t> &mask) 
        {
            Word * const H = slab.ldz().as<Word>(Bins);
            for(size_t k=slab.count();k>0;--k)
            {
                const Ink::HSegment    &seg = slab.hseg[k];
                const PixRow<uint8_t>  &msk = mask[seg.y];
                for(unit_t i=seg.w,x=seg.x;i>0;--i,++x)
                {
                    ++H[msk(x)];
                }
            }
        }

#if 0

        namespace Crux
        {
            Histogram:: ~Histogram() noexcept
            {
            }

            Histogram:: Histogram() noexcept : lower(0), upper(0)
            {
            }
            
            void Histogram:: ldz_(void *const blocks, const size_t blockSize) const noexcept
            {
                assert(0!=blocks);
                assert(blockSize>0);
                memset(blocks,0,blockSize*BINS);
                Coerce(lower) = 0;
                Coerce(upper) = BINS-1;
            }
            
        }
#endif

    }

}

