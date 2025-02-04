#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"
#include "y/type/utils.hpp"
#include "y/dft/dft.hpp"
#include "y/system/exception.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{
    namespace Apex
    {


        namespace
        {
            typedef double Real;
            typedef Complex<Real> Cplx;

            template <typename T>
            class BlockOf
            {
            public:
                explicit BlockOf(const size_t n) :
                block(n*sizeof(T),AsCapacity),
                size(n),
                item( static_cast<T *>(block->entry) - 1)
                {
                    assert(block->range>=n*sizeof(T));
                    assert( IsPowerOfTwo(n) );
                }

                inline T & operator[](const size_t indx) noexcept
                {
                    assert(indx>=1);
                    assert(indx<=size);
                    return item[indx];
                }

                inline T * operator()(void) noexcept { return item; }



            private:
                BlockPtr     block;
            public:
                const size_t size;
                T * const    item; // [1..size]
            private:
                Y_DISABLE_COPY_AND_ASSIGN(BlockOf);
            };
        }

        
        Block * Natural:: FFT(Block &          lhs,
                              Block &          rhs,
                              uint64_t * const ell)
        {
            static Factory & factory = Factory::Instance();
            const Jig1 & u = lhs.make<Plan1>(); const size_t n = u.words; if(n<=0) return factory.query(0);
            const Jig1 & v = rhs.make<Plan1>(); const size_t m = v.words; if(m<=0) return factory.query(0);

            const bool   watch = 0!=ell;

            //------------------------------------------------------------------
            //
            //
            // find nn to emcompass product length
            //
            //
            //------------------------------------------------------------------
            size_t       nn    = 2; // to use RealTransform
            {
                const size_t mx    = Max(n,m);
                while (nn < mx)
                    nn <<= 1;
                nn <<= 1;
            }

            uint64_t ini = 0;
            BlockOf<Real> b(nn);
            {
                BlockOf<Real> a(nn);
                if(watch) ini = WallTime::Ticks();

                //--------------------------------------------------------------
                //
                // load data
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) a[i] = u.word[n-i];
                for(size_t i=m;i>0;--i) b[i] = v.word[m-i];


                Yttrium::DFT::RealTransform(a.item, nn, 1);
                Yttrium::DFT::RealTransform(b.item, nn, 1);

                //--------------------------------------------------------------
                //
                // product
                //
                //--------------------------------------------------------------
                b[1] *= a[1];
                b[2] *= a[2];
                for(size_t j=3;j<=nn;j+=2) {
                    const size_t j1 = j+1;
                    const Real   t  = b[j];
                    b[j]  = t*a[j]  - b[j1]*a[j1];
                    b[j1] = t*a[j1] + b[j1]*a[j];
                }
            }

            //------------------------------------------------------------------
            //
            // reverse
            //
            //------------------------------------------------------------------
            Yttrium::DFT::RealTransform(b.item,nn,-1);



            const Real RX=256.0;
            Real       cy=0.0;
            for(size_t j=nn;j>=1;--j) {
                const Real t= floor(b[j]/(nn>>1)+cy+0.5);
                cy=(unsigned long) (t/RX);
                b[j]=t-cy*RX;
            }
            if (cy >= RX) throw Specific::Exception("Apex::FFT","precision underflow");


            if(watch)      *ell += WallTime::Ticks() - ini;
            const size_t    mpn  = m+n;
            Block *         prod = factory.queryBytes(mpn);
            {
                uint8_t * const w = prod->make<Plan1>().word-1;
                w[mpn] = (uint8_t)cy;
                for(size_t j=1;j<mpn;++j)
                {
                    w[j] = (uint8_t)b[mpn-j];
                }
            }
            prod->sync();
            return prod;
        }
    }
}


namespace Yttrium
{
    namespace Apex
    {
        Natural Natural:: FFT(const Natural &lhs, const Natural &rhs)
        {
            Y_Apex_Lock(lhs);
            Y_Apex_Lock(rhs);
            return Natural( FFT(*lhs.block,*rhs.block,0),AsBlock);
        }
    }
}
