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
        
        long double Histogram:: computeWeightedVariance(const unsigned lower, const unsigned upper) const
        {
            size_t num = 0;
            size_t sum = 0;
            for(size_t i=lower;i<upper;++i)
            {
                const size_t n = data[i];
                num += n;
                sum += n * i;
            }
            if(num>0)
            {
                const long double proba    = static_cast<long double>(num)/total;
                const long double average  = static_cast<long double>(sum)/num;
                long double       variance = 0;
                for(size_t i=lower;i<upper;++i)
                {
                    const long double n = static_cast<long double>(data[i]);
                    const long double d = static_cast<long double>(i) - average;
                    variance += n * d*d;
                }
                variance /= num;
                return proba * variance;
            }
            else
            {
                return 0;
            }
        }

        long double Histogram:: fetchVarianceSeparation(const uint8_t t) const {
            return computeWeightedVariance(0,t) + computeWeightedVariance(t,255);
        }

        uint8_t Histogram:: threshold() const
        {
            unsigned    imin  = 0;
            long double smin = fetchVarianceSeparation(0);

            for(unsigned i=1;i<=255;++i)
            {
                const long double stmp = fetchVarianceSeparation(i);
                if(stmp<=smin)
                {
                    imin = i;
                    smin = stmp;
                }
            }
            return static_cast<uint8_t>(imin);
        }

    }

}

