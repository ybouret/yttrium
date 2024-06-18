
#include "y/kemp/element/fft-mul.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/fft/fft.hpp"
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        const Element::BinaryAPI Element:: MulFFT =
        {
            FFT_Multiplication:: Result,
            FFT_Multiplication:: ResTMX,
            FFT_Multiplication:: ResL64,
            FFT_Multiplication:: ResR64
        };

        Element * FFT_Multiplication:: Get(const Assembly<uint8_t> & lhs,
                                           const Assembly<uint8_t> & rhs,
                                           uint64_t * const          tmx)
        {

            //__________________________________________________________________
            //
            //
            // get array of bytes
            //
            //__________________________________________________________________
            const size_t n = lhs.positive;
            const size_t m = rhs.positive;

            if(n<=0||m<=0) return Element::Zero();

            //__________________________________________________________________
            //
            //
            // get common size, extend to avoid overlapping
            //
            //__________________________________________________________________
            size_t       nn = 1;
            unsigned     ns = 0;
            const size_t mn = Max(n,m);
            while (nn < mn)  { nn <<= 1; ++ns; }
            nn <<= 1; ++ns;


            const bool       tag    = 0!=tmx;
            const size_t     mpn    = m+n;
            AutoPtr<Element> P      = new Element(mpn,AsCapacity);
            const uint64_t   mark64 = tag ? WallTime::Ticks() : 0;

            {
                typedef double        Real;
                static const unsigned Log2RealSize = iLog2< sizeof(Real) >::Value;

                //--------------------------------------------------------------
                //
                // acquire local memory : 2 arrays of nn reals
                //
                //--------------------------------------------------------------
                unsigned     shift = 1 + (ns+Log2RealSize); assert( (1<<shift)/sizeof(Real) == 2*nn );
                void * const entry = Memory::Archon::Acquire(shift);
                Real * const b     = static_cast<Real *>(entry)-1;
                Real * const a     = b + nn;

                //--------------------------------------------------------------
                //
                // fill arrays
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) a[i] = lhs.item[n-i];
                for(size_t i=m;i>0;--i) b[i] = rhs.item[m-i];

                //--------------------------------------------------------------
                //
                // forward dual FFT
                //
                //--------------------------------------------------------------
                FFT::ForwardReal(a,b,nn);

                //--------------------------------------------------------------
                //
                // FFTs multiplication
                //
                //--------------------------------------------------------------
                b[1] *= a[1];
                b[2] *= a[2];
                for(size_t j=3;j<=nn;j+=2)
                {
                    const size_t j1 = j+1;
                    const Real   t  = b[j];
                    b[j]  =t*a[j]-b[j1]*a[j1];
                    b[j1] =t*a[j1]+b[j1]*a[j];
                }

                //--------------------------------------------------------------
                //
                // reverse FFT
                //
                //--------------------------------------------------------------
                FFT::ReverseReal(b,nn);


                static const Real RX(256);
                const size_t      nh = nn>>1;
                static const Real half(0.5);

                //--------------------------------------------------------------
                //
                // Retrieving coefficients and carry
                //
                //--------------------------------------------------------------
                Real cy = 0;
                for(size_t j=nn;j>=1;--j)
                {
                    const Real t =  floor(b[j]/nh+cy+half);
                    cy=(unsigned long) (t/RX);
                    *(uint8_t *)&b[j]= static_cast<uint8_t>(t-cy*RX);
                }

                //--------------------------------------------------------------
                //
                // little endian transfer
                //
                //--------------------------------------------------------------
                {
                    uint8_t * w = P->bytes.item+mpn;
                    *(--w) = uint8_t( cy );
                    for (size_t j=1;j<mpn;++j)
                        *(--w) = *(const uint8_t *) &b[j];
                }



                //--------------------------------------------------------------
                //
                //  set bytes metrics
                //
                //--------------------------------------------------------------
                P->bytes.positive = mpn;
                P->bits           = P->bytes.updateBits();
                Memory::Archon::Release(entry,shift);
            }

            if(tag) *tmx += (WallTime::Ticks() - mark64);
            return P.yield()->revise();
        }


    }

}
