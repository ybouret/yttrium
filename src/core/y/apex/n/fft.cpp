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

                //! load u[0..n-1]
                template <typename U> inline
                void load(U *u, const size_t n) noexcept
                {
                    T *target = item;
                    for(size_t i=n;i>0;--i)
                    {
                        *(++target) = *(u++);
                    }
                }

            private:
                BlockPtr     block;
            public:
                const size_t size;
                T * const    item; // [1..size]
            private:
                Y_DISABLE_COPY_AND_ASSIGN(BlockOf);
            };
        }


        static inline String Real2Hex(const Real x)
        {
            return Formatted::Get("0x%x", unsigned( floor(x+0.5)) );
        }

        

        Block * Block:: FFT(Block &lhs, Block &rhs)
        {
            static Factory & factory = Factory::Instance();
            const Jig1 & u = lhs.make<Plan1>();
            const size_t n = u.words; if(n<=0) return factory.query(0);
            const Jig1 & v = rhs.make<Plan1>();
            const size_t m = v.words; if(m<=0) return factory.query(0);

            const size_t mx = Max(n,m);
            size_t       nn = 1;
            while (nn < mx)
                nn <<= 1;
            nn <<= 1; // for product size

            BlockOf<Real> a(nn);
            BlockOf<Real> b(nn);

            //a.load(u.word,n);
            //b.load(v.word,m);
            for(size_t i=n;i>0;--i) a[i] = u.word[n-i];
            for(size_t i=m;i>0;--i) b[i] = v.word[m-i];

            std::cerr << "u=" << u << std::endl;
            std::cerr << "v=" << v << std::endl;

            Core::Display(std::cerr << "a=", &a[1], nn, Real2Hex ) << std::endl;
            Core::Display(std::cerr << "b=", &b[1], nn, Real2Hex ) << std::endl;

            Yttrium::DFT::RealTransform(a.item, nn, 1);
            Yttrium::DFT::RealTransform(b.item, nn, 1);

            b[1] *= a[1];
            b[2] *= a[2];
            for(size_t j=3;j<=nn;j+=2) {
                const Real t = b[j];
                b[j]  =t*a[j]-b[j+1]*a[j+1];
                b[j+1]=t*a[j+1]+b[j+1]*a[j];
            }
            Yttrium::DFT::RealTransform(b.item,nn,-1);
            Core::Display(std::cerr << "b=", &b[1], nn) << std::endl;


            const Real RX=256.0;
            Real       cy=0.0;
            for(size_t j=nn;j>=1;--j) {
                const Real t= floor(b[j]/(nn>>1)+cy+0.5);
                cy=(unsigned long) (t/RX);
                b[j]=t-cy*RX;
            }
            if (cy >= RX) throw Specific::Exception("Apex::DFT","precision underflow");
            Core::Display(std::cerr << "b=", &b[1], nn) << std::endl;
            std::cerr << "cy=" << cy << std::endl;

            const size_t    mpn = m+n;
            Block *         prod = factory.queryBytes(mpn);
            {
                uint8_t * const w = prod->make<Plan1>().word-1;
                w[mpn] = (uint8_t)cy;
                for(size_t j=1;j<mpn;++j)
                {
                    w[j] = (uint8_t)b[mpn-j];
                }

                //w[0] = (uint8_t)cy;
                //for (size_t j=1;j<mpn;j++)
                  //  w[j]=(uint8_t) b[j];

            }
            prod->sync();
            std::cerr << "p=" << *prod << std::endl;
#if 0
            w[1]=(unsigned char) cy; Copy answer to output.
            for (j=2;j<=n+m;j++)
                w[j]=(unsigned char) b[j-1];
#endif

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
            return Natural(Block::FFT(*lhs.block,*rhs.block),AsBlock);
        }
    }
}
