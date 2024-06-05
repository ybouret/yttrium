#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"
#include <cstring>
#include <cmath>
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"


#include "y/kemp/element/fft.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;

Y_UTEST(kemp_element)
{
    Random::ParkMiller ran;
    size_t             cycles=1;

    if(argc>1) cycles = ASCII::Convert::To<size_t>(argv[1],"cycles");

    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(BitCount::For(qw)==i);

        { uint64_t temp = qw; const Kemp::Assembly<uint8_t>  a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const Kemp::Assembly<uint16_t> a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const Kemp::Assembly<uint32_t> a(temp); std::cerr << a << std::endl; }
        { uint64_t temp = qw; const Kemp::Assembly<uint64_t> a(temp); std::cerr << a << std::endl; }
    }


    for(size_t bits=0;bits<=80;++bits)
    {
        std::cerr << std::setw(3) << bits;
        std::cerr << " : " <<  std::setw(3) << Kemp::Assembly<uint8_t>:: BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << Kemp::Assembly<uint16_t>::BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << Kemp::Assembly<uint32_t>::BitsToPositive(bits);
        std::cerr << " : " <<  std::setw(3) << Kemp::Assembly<uint64_t>::BitsToPositive(bits);
        std::cerr << std::endl;
    }

    std::cerr << "2^bits" << std::endl;
    for(size_t bits=0;bits<=80;++bits)
    {
        const Kemp::Element el(Kemp::TwoToThe,bits);
        std::cerr << el << std::endl;
    }

    std::cerr << "<ToNum64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t qw = ran.to<uint64_t>(i); Y_ASSERT(i==BitCount::For(qw));
        Kemp::Element  el(qw,Kemp::ToNum64);
        Y_ASSERT(el.bits == i);
        Y_ASSERT(Kemp::AsNum64 == el.state);
        for(unsigned k=0;k<4;++k)
        {
            for(unsigned l=0;l<4;++l)
            {
                Y_ASSERT( el.set( Kemp::Element::Inner[k]).u64() == qw );
                Y_ASSERT( el.set( Kemp::Element::Inner[l]).u64() == qw );
            }
        }

    }

    std::cerr << "<Random>" << std::endl;
    for(unsigned i=0;i<=1024;++i)
    {
        Kemp::Element    el(i,ran);
        Vector<uint8_t>  org(el.bytes.capacity,AsCapacity);
        for(size_t j=0;j<el.bytes.capacity;++j) {
            org << el.bytes.item[j];
        }
        Y_ASSERT(org.size()==el.measure());

        for(unsigned k=0;k<4;++k)
        {
            for(unsigned l=0;l<4;++l)
            {
                (void) el.set( Kemp::Element::Inner[k]);
                (void) el.set( Kemp::Element::Inner[l]);
                el.set(Kemp::AsBytes);
                Y_ASSERT( 0 == memcmp( &org[1], el.ro_addr(), el.measure() ) );
            }
        }
    }


    std::cerr << "<Compare64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            const uint64_t lhs = ran.to<uint64_t>(i);
            const uint64_t rhs = ran.to<uint64_t>(j);
            const SignType cmp = Sign::Of(lhs,rhs);

            Kemp::Element  L(lhs, Kemp::ToNum64);
            Kemp::Element  R(rhs, Kemp::ToNum64);

            for(unsigned k=0;k<4;++k)
            {
                for(unsigned l=0;l<4;++l)
                {
                    L.set( Kemp::Element::Inner[k]);
                    R.set( Kemp::Element::Inner[l]);
                    Y_ASSERT( cmp == Kemp::Element::Compare(L,R) );
                    L.set( Kemp::Element::Inner[k]);
                    R.set( Kemp::Element::Inner[l]);
                    Y_ASSERT( cmp == Kemp::Element::Compare(lhs,R) );
                    Y_ASSERT( cmp == Kemp::Element::Compare(L,rhs) );
                }
            }
        }
    }

    WallTime chrono;
    size_t   total = 0;
    uint64_t tmx[Kemp::Element::Kinds] = { 0 };

    std::cerr << "<Add64>" << std::endl;

    total = 0;
    memset(tmx,0,sizeof(tmx));
    for(unsigned i=0;i<=63;++i)
    {
        for(unsigned j=0;j<=63;++j)
        {
            for(size_t cycle=0;cycle<cycles;++cycle)
            {
                ++total;

                const uint64_t l = ran.to<uint64_t>(i);
                const uint64_t r = ran.to<uint64_t>(j);
                const uint64_t s = l+r; Y_ASSERT(s-l==r); Y_ASSERT(s-r==l);
                const size_t   b = BitCount::For(s);

                Kemp::Element          L(l, Kemp::ToNum64); Y_ASSERT(L.bits==i); Y_ASSERT(L.u64()==l);
                Kemp::Element          R(r, Kemp::ToNum64); Y_ASSERT(R.bits==j); Y_ASSERT(R.u64()==r);

                for(unsigned k=0;k<Kemp::Element::Kinds;++k)
                {
                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Add[k](L,R);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==s);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Add[k](l,R);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==s);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Add[k](L,r);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==s);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Add[k](L,R,tmx[k]);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==s);
                    }


                }

            }

        }

    }



#define RATE(VAR) std::cerr << std::setw(20) <<  #VAR << " = " << HumanReadable(ceil(static_cast<long double>(total)/chrono(VAR))) << "op/s" << std::endl
#define RATES()            \
RATE(tmx[Kemp::Ops64_32]); \
RATE(tmx[Kemp::Ops64_16]); \
RATE(tmx[Kemp::Ops64_8]);  \
RATE(tmx[Kemp::Ops32_16]); \
RATE(tmx[Kemp::Ops32_8]);  \
RATE(tmx[Kemp::Ops16_8])

    RATES(); std::cerr << std::endl;


    std::cerr << "<Sub64>" << std::endl;
    total = 0;
    memset(tmx,0,sizeof(tmx));
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            for(size_t cycle=0;cycle<cycles;++cycle)
            {
                ++total;
                uint64_t lhs = ran.to<uint64_t>(i);
                uint64_t rhs = ran.to<uint64_t>(j);
                if(lhs<rhs) Swap(lhs,rhs);
                const uint64_t sub = lhs-rhs;
                const size_t b = BitCount::For(sub);

                Kemp::Element L(lhs,Kemp::ToNum64);
                Kemp::Element R(rhs,Kemp::ToNum64);

                for(unsigned k=0;k<Kemp::Element::Kinds;++k)
                {
                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Sub[k](L,R);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==sub);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Sub[k](lhs,R);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==sub);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Sub[k](L,rhs);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==sub);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Sub[k](L,R,tmx[k]);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==sub);
                    }
                }


            }
        }
    }
    RATES(); std::cerr << std::endl;

    std::cerr << "<Mul64>" << std::endl;


    total = 0;
    memset(tmx,0,sizeof(tmx));
    for(unsigned i=0;i<=32;++i)
    {
        for(unsigned j=0;j<=32;++j)
        {
            for(size_t cycle=0;cycle<cycles;++cycle)
            {
                ++total;
                uint64_t lhs = ran.to<uint64_t>(i);
                uint64_t rhs = ran.to<uint64_t>(j);
                const uint64_t mul = lhs*rhs;
                const size_t b = BitCount::For(mul);

                Kemp::Element L(lhs,Kemp::ToNum64);
                Kemp::Element R(rhs,Kemp::ToNum64);
                for(unsigned k=0;k<Kemp::Element::Kinds;++k)
                {
                    {
                        AutoPtr<Kemp::Element> P = Kemp::Element::Mul[k](L,R);
                        Y_ASSERT(P->bits==b);
                        Y_ASSERT(P->u64()==mul);
                    }

                    {
                        AutoPtr<Kemp::Element> P = Kemp::Element::Mul[k](lhs,R);
                        Y_ASSERT(P->bits==b);
                        Y_ASSERT(P->u64()==mul);
                    }

                    {
                        AutoPtr<Kemp::Element> P = Kemp::Element::Mul[k](L,rhs);
                        Y_ASSERT(P->bits==b);
                        Y_ASSERT(P->u64()==mul);
                    }

                    {
                        AutoPtr<Kemp::Element> S = Kemp::Element::Mul[k](L,R,tmx[k]);
                        Y_ASSERT(S->bits==b);
                        Y_ASSERT(S->u64()==mul);
                    }
                }
            }
        }
    }
    RATES(); std::cerr << std::endl;


    std::cerr << "<SHR>" << std::endl;
    for(unsigned k=0;k<4;++k)
    {
        Kemp::Element x(Kemp::TwoToThe,80);
        x.set(Kemp::Element::Inner[k]);
        while(x.bits>0)
        {
            const size_t nextBits = x.bits-1;
            x.shr();
            Y_ASSERT(nextBits==x.bits);
        }
    }

    std::cerr << "<SHR64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t l = ran.to<uint64_t>(i);
        Kemp::Element  L(l,Kemp::ToNum64);
        for(size_t j=0;j<64;++j)
        {
            const uint64_t s = l>>j;
            //std::cerr << Hexadecimal(l) << ">>" << j << " = " << Hexadecimal(s) << std::endl;
            AutoPtr<Kemp::Element> S = L.shr(j);
            //std::cerr << S << std::endl;
            Y_ASSERT(S->u64() == s );
        }
    }

    std::cerr << "<SHL64>" << std::endl;
    for(unsigned i=0;i<64;++i)
    {
        const uint64_t l = ran.to<uint64_t>(i);
        Kemp::Element  L(l,Kemp::ToNum64);
        for(unsigned j=0;j<=64-i;++j)
        {
            const uint64_t s = l<<j;
            //std::cerr << Hexadecimal(l) << "<<" << j << " = " << Hexadecimal(s);
            AutoPtr<Kemp::Element> S = L.shl(j);
            if(i>0)
            {
                Y_ASSERT(i+j==BitCount::For(s));
            }
            else
            {
                Y_ASSERT(0==s);
            }
            //std::cerr << " => " << S << std::endl;
            Y_ASSERT(S->u64() == s);
        }
    }

    std::cerr << "<FFT64>" << std::endl;

    total = 0;
    uint64_t tmx_fft = 0;
    for(unsigned i=0;i<=32;++i)
    {
        for(unsigned j=0;j<=32;++j)
        {
            for(size_t cycle=0;cycle<cycles;++cycle)
            {
                ++total;
                uint64_t       lhs = ran.to<uint64_t>(i);
                uint64_t       rhs = ran.to<uint64_t>(j);
                const uint64_t prod = lhs*rhs;
                const size_t   bits = BitCount::For(prod);

                //std::cerr << "prod=" << Hexadecimal(prod,Hexadecimal::Compact) << std::endl;

                Kemp::Element L(lhs,Kemp::ToNum64);
                Kemp::Element R(rhs,Kemp::ToNum64);

                {
                    AutoPtr<const Kemp::Element> P = Kemp::Element::MulFFT(L,R);
                    Y_ASSERT(bits == P->bits);
                    Y_ASSERT(P->u64() == prod);
                }

                {
                    AutoPtr<const Kemp::Element> P = Kemp::Element::MulFFT(L,R,tmx_fft);
                    Y_ASSERT(bits == P->bits);
                    Y_ASSERT(P->u64() == prod);
                }


                {
                    AutoPtr<const Kemp::Element> P = Kemp::Element::MulFFT(lhs,R);
                    Y_ASSERT(bits == P->bits);
                    Y_ASSERT(P->u64() == prod);
                }

                {
                    AutoPtr<const Kemp::Element> P = Kemp::Element::MulFFT(L,rhs);
                    Y_ASSERT(bits == P->bits);
                    Y_ASSERT(P->u64() == prod);
                }

            }
        }
    }
    RATE(tmx_fft);


    std::cerr << "<Bitwise64>" << std::endl;
    for(unsigned i=0;i<64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            const uint64_t l = ran.to<uint64_t>(i);
            const uint64_t r = ran.to<uint64_t>(j);
            Kemp::Element  L(l,Kemp::ToNum64);
            Kemp::Element  R(r,Kemp::ToNum64);

            {
                const uint64_t x = l|r;
                for(unsigned k=0;k<4;++k)
                {
                    for(unsigned l=0;l<4;++l)
                    {
                        L.set( Kemp::Element::Inner[k]);
                        L.set( Kemp::Element::Inner[l]);
                        AutoPtr<Kemp::Element> X = Kemp::Element::Bitwise(Kemp::OR,L,R);
                        Y_ASSERT(X->u64() == x);
                    }
                }
            }

            {
                const uint64_t x = l&r;
                for(unsigned k=0;k<4;++k)
                {
                    for(unsigned l=0;l<4;++l)
                    {
                        L.set( Kemp::Element::Inner[k]);
                        L.set( Kemp::Element::Inner[l]);
                        AutoPtr<Kemp::Element> X = Kemp::Element::Bitwise(Kemp::AND,L,R);
                        Y_ASSERT(X->u64() == x);
                    }
                }
            }

            {
                const uint64_t x = l^r;
                for(unsigned k=0;k<4;++k)
                {
                    for(unsigned l=0;l<4;++l)
                    {
                        L.set( Kemp::Element::Inner[k]);
                        L.set( Kemp::Element::Inner[l]);
                        AutoPtr<Kemp::Element> X = Kemp::Element::Bitwise(Kemp::XOR,L,R);
                        Y_ASSERT(X->u64() == x);
                    }
                }
            }

        }

    }

    std::cerr << "<Square64>" << std::endl;
    total = 0;
    memset(tmx,0,sizeof(tmx));
    for(unsigned i=0;i<=32;++i)
    {
        for(size_t cycle=0;cycle<=cycles;++cycle)
        {
            ++total;
            const uint64_t arg = ran.to<uint64_t>(i);
            const uint64_t sqr = arg*arg;
            Kemp::Element  Arg(arg,Kemp::ToNum64);

            for(unsigned k=0;k<Kemp::Element::Kinds;++k)
            {
                AutoPtr<Kemp::Element> S = Kemp::Element::Sqr[k](Arg);
                Y_ASSERT(S->u64()==sqr);
            }

            for(unsigned k=0;k<Kemp::Element::Kinds;++k)
            {
                AutoPtr<Kemp::Element> S = Kemp::Element::Sqr[k](Arg,tmx[k]);
                Y_ASSERT(S->u64()==sqr);
            }
        }
    }
    RATES(); std::cerr << std::endl;


    std::cerr << std::endl;
    Y_SIZEOF(Kemp::Bytes);
    Y_SIZEOF(Kemp::Num16);
    Y_SIZEOF(Kemp::Num32);
    Y_SIZEOF(Kemp::Num64);
    Y_SIZEOF(Kemp::Element);



}
Y_UDONE()

