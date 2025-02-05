#include "y/apex/natural.hpp"
#include "y/apex/block/factory.hpp"
#include "y/type/utils.hpp"
#include "y/dft/dft.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {


        namespace
        {
            typedef double        Real;
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
                              Block &          rhs)
        {
            static Factory & factory = Factory::Instance();
            const Jig1 & u = lhs.make<Plan1>(); const size_t n = u.words; if(n<=0) return factory.query(0);
            const Jig1 & v = rhs.make<Plan1>(); const size_t m = v.words; if(m<=0) return factory.query(0);


            //------------------------------------------------------------------
            //
            //
            // find nn to emcompass product length
            //
            //
            //------------------------------------------------------------------
            size_t       nn    = 2; // minimal to use RealTransform
            size_t       nmul  = 0; // nn/2 - 1
            {
                const size_t mx = Max(n,m);
                while (nn < mx)
                    nn <<= 1;
                nmul  = nn;
                nn <<= 1;
                --nmul;
            }

            BlockOf<Real> b(nn);
            {
                BlockOf<Real> a(nn);

                //--------------------------------------------------------------
                //
                // load data as byte
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) a[i] = u.word[n-i];
                for(size_t i=m;i>0;--i) b[i] = v.word[m-i];


#if 0
                DFT::RealForward(a.item, nn);
                DFT::RealForward(b.item, nn);
#else
                DFT::RealForward(a.item,b.item,nn);
#endif

                //--------------------------------------------------------------
                //
                // product
                //
                //--------------------------------------------------------------
                b[1] *= a[1];
                b[2] *= a[2];
                {
                    Cplx *      lhs = (Cplx *)       &b[1];
                    const Cplx *rhs = (const Cplx *) &a[1];
                    for(size_t j=nmul;j>0;--j) {
                        *(++lhs) *= *(++rhs);
                    }

                }
            }

            //------------------------------------------------------------------
            //
            // reverse
            //
            //------------------------------------------------------------------
            DFT::RealReverse(b.item,nn);



            Real       carry  = 0.0;
            const Real denom = (nn>>1);
            for(size_t j=nn;j>0;--j)
            {
                const Real t = floor(b[j]/denom+carry+0.5);
                carry=(unsigned long) (t*0.00390625);
                *(uint8_t *) &b[j] = static_cast<uint8_t>(t-carry*256.0);
            }
            if (carry >= 256.0) throw Specific::Exception("Apex::FFT","precision underflow");


            const size_t    mpn  = m+n;
            Block *         prod = factory.queryBytes(mpn);
            {
                uint8_t * const w = prod->make<Plan1>().word-1;
                w[mpn] = (uint8_t)carry;
                for(size_t j=1;j<mpn;++j)
                {
                    w[j] = *(const uint8_t *) &b[mpn-j];
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
            return Natural(FFT(*lhs.block,*rhs.block),AsBlock);
        }
    }
}
