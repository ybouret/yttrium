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

            
            BlockPtr     blk( factory.queryBytes( (nn << 1) * sizeof(Real)  ) ); assert(blk->range >= (nn<<1) * sizeof(Real) );
            Real * const b = static_cast<Real *>(blk->entry) - 1;
            {
                Real * const a = b+nn;

                //--------------------------------------------------------------
                //
                // load data as byte
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) a[i] = u.word[n-i];
                for(size_t i=m;i>0;--i) b[i] = v.word[m-i];

                //--------------------------------------------------------------
                //
                // Dual Real DFT
                //
                //--------------------------------------------------------------
                DFT::RealForward(a,b,nn);

                //--------------------------------------------------------------
                //
                // in-place product
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
            DFT::RealReverse(b,nn);


            //------------------------------------------------------------------
            //
            // retrieve hexadecimal decription
            //
            //------------------------------------------------------------------
            Real       carry  = 0.0;
            const Real denom = (nn>>1);
            for(size_t j=nn;j>0;--j)
            {
                Real      &   B = b[j];
                const Real    t = floor(B/denom+carry+0.5);
                carry           = (unsigned long) (t*0.00390625);
                *(uint8_t *)& B = static_cast<uint8_t>(t-carry*256.0); //!< store directly
            }
            if (carry >= 256.0) throw Specific::Exception("Apex::FFT","precision underflow");


            //------------------------------------------------------------------
            //
            // transfer to final block
            //
            //------------------------------------------------------------------
            const size_t    mpn  = m+n;
            Block *         prod = factory.queryBytes(mpn);
            {
                uint8_t * const w = prod->make<Plan1>().word-1;
                w[mpn] = (uint8_t)carry;
                for(size_t j=1;j<mpn;++j)
                    w[j] = *(const uint8_t *) &b[mpn-j];
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
