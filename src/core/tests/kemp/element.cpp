#include "y/kemp/element.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/sequence/vector.hpp"
#include <cstring>
#include <cmath>
#include "y/ptr/auto.hpp"
#include "y/system/wtime.hpp"
#include "y/text/human-readable.hpp"

using namespace Yttrium;

Y_UTEST(kemp_element)
{
    Random::ParkMiller ran;
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

    std::cerr << "<Add64>" << std::endl;
    uint64_t tmx64_32 = 0;
    uint64_t tmx64_16 = 0;
    uint64_t tmx64_8  = 0;
    uint64_t tmx32_16 = 0;
    uint64_t tmx32_8  = 0;
    uint64_t tmx16_8  = 0;

    size_t total = 0;
    for(unsigned i=0;i<=63;++i)
    {
        for(unsigned j=0;j<=63;++j)
        {
            for(size_t cycle=0;cycle<128;++cycle)
            {
                ++total;

                const uint64_t l = ran.to<uint64_t>(i);
                const uint64_t r = ran.to<uint64_t>(j);
                const uint64_t s = l+r; Y_ASSERT(s-l==r); Y_ASSERT(s-r==l);
                const size_t   b = BitCount::For(s);
                
                Kemp::Element          L(l, Kemp::ToNum64); Y_ASSERT(L.bits==i); Y_ASSERT(L.u64()==l);
                Kemp::Element          R(r, Kemp::ToNum64); Y_ASSERT(R.bits==j); Y_ASSERT(R.u64()==r);
                {
                    L.set(Kemp::AsNum32);
                    R.set(Kemp::AsNum32);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint64_t,uint32_t>(L,R,&tmx64_32);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }
                
                {
                    L.set(Kemp::AsNum16);
                    R.set(Kemp::AsNum16);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint64_t,uint16_t>(L,R,&tmx64_16);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }
                
                {
                    L.set(Kemp::AsBytes);
                    R.set(Kemp::AsBytes);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint64_t,uint8_t>(L,R,&tmx64_8);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }


                {
                    L.set(Kemp::AsNum16);
                    R.set(Kemp::AsNum16);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint32_t,uint16_t>(L,R,&tmx32_16);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }

                {
                    L.set(Kemp::AsBytes);
                    R.set(Kemp::AsBytes);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint32_t,uint8_t>(L,R,&tmx32_8);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }

                {
                    L.set(Kemp::AsBytes);
                    R.set(Kemp::AsBytes);
                    AutoPtr<Kemp::Element> S = Kemp::Element::Add<uint16_t,uint8_t>(L,R,&tmx16_8);
                    Y_ASSERT(S->bits==b);
                    Y_ASSERT(S->u64()==s);
                }

            }

        }

    }

    WallTime chrono;

#define RATE(VAR) std::cerr << std::setw(12) <<  #VAR << " = " << HumanReadable(ceil(static_cast<long double>(total)/chrono(VAR))) << std::endl

    RATE(tmx64_32);
    RATE(tmx64_16);
    RATE(tmx64_8 );
    RATE(tmx32_16);
    RATE(tmx32_8 );
    RATE(tmx16_8 );




    Y_SIZEOF(Kemp::Bytes);
    Y_SIZEOF(Kemp::Num16);
    Y_SIZEOF(Kemp::Num32);
    Y_SIZEOF(Kemp::Num64);
    Y_SIZEOF(Kemp::Element);



}
Y_UDONE()

