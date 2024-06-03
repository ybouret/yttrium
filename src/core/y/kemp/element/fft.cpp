
#include "y/kemp/element/fft.hpp"
#include "y/type/utils.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/fft/fft.hpp"

namespace Yttrium
{
    namespace Kemp
    {


        Element * FFTMul:: Get( Element &lhs, Element &rhs )
        {
            
            // get array of bytes
            const Assembly<uint8_t> & u = lhs.get<uint8_t>();
            const size_t              n = u.positive;
            const Assembly<uint8_t> & v = rhs.get<uint8_t>();
            const size_t              m = v.positive;

            if(n<=0||m<=0) return new Element(0,AsCapacity);


            // get common size, extend to avoid overlapping
            size_t       nn = 1;
            unsigned     ns = 0;
            const size_t mn = Max(n,m);
            while (nn < mn)  { nn <<= 1; ++ns; }
            nn <<= 1; ++ns;


            const size_t mpn = m+n;
            AutoPtr<Element> P = new Element(mpn,AsCapacity);
            {
                typedef double        Real;
                static const unsigned Log2RealSize = iLog2< sizeof(Real) >::Value;

                //--------------------------------------------------------------
                //
                // acquire local memory
                //
                //--------------------------------------------------------------
                unsigned              shift = 1 + (ns+Log2RealSize); assert( (1<<shift)/sizeof(Real) == 2*nn );
                void * const          entry = Memory::Archon::Acquire(shift);
                Real * const b = static_cast<Real *>(entry)-1;
                Real * const a = b + nn;

                //--------------------------------------------------------------
                //
                // fill arrays
                //
                //--------------------------------------------------------------
                for(size_t i=n;i>0;--i) a[i] = u.item[n-i];
                for(size_t i=m;i>0;--i) b[i] = v.item[m-i];

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
                    *(uint8_t *)&b[j]=uint8_t(t-cy*RX);
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
                    Hexadecimal::Display(std::cerr << "w=", w, mpn) << std::endl;
                }

                //--------------------------------------------------------------
                //
                //  set bytes metrics
                //
                //--------------------------------------------------------------
                P->bytes.positive = mpn;
                P->bits = P->bytes.updateBits();
                Memory::Archon::Release(entry,shift);
            }

            return P.yield()->revise();
        }


    }

}
