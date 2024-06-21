#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/bit-count.hpp"

using namespace Yttrium;

Y_UTEST(kemp_natural)
{
    Random::ParkMiller ran;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t lhs = ran.to<uint64_t>(i);
        apn            L(lhs);
        std::cerr << Hexadecimal(lhs,Hexadecimal::Compact) << "/" << L << std::endl;
    }

    for(unsigned i=0;i<=80;++i)
    {
        const apn n(Kemp::Exp2,i);
        std::cerr << n << std::endl;
    }

    std::cerr << "<Add/Cmp64>" << std::endl;
    for(unsigned i=0;i<=63;++i)
    {
        for(unsigned j=0;j<=63;++j)
        {
            const uint64_t lhs = ran.to<uint64_t>(i);
            const uint64_t rhs = ran.to<uint64_t>(j);
            const uint64_t sum = lhs+rhs;
            {
                apn L = lhs; Y_ASSERT(L==lhs);
                apn R = rhs; Y_ASSERT(R==rhs);
                Y_ASSERT( Sign::Of(lhs,rhs) == Sign::Of(L,R) );
                {
                    L += R; Y_ASSERT(L==sum);
                }
                {
                    L = lhs;
                    R += L; Y_ASSERT(R==sum);
                }
                {
                    L = lhs;
                    R = rhs;
                    { apn S = L+R;   Y_ASSERT(S==sum); }
                    { apn S = lhs+R; Y_ASSERT(S==sum); }
                    { apn S = L+rhs; Y_ASSERT(S==sum); }

                }
            }
        }
    }
    
    for(apn i=0;i<=10;++i) std::cerr << i << "/";
    std::cerr << std::endl;
    for(apn i=0;i<=10;i++) std::cerr << i << "/";
    std::cerr << std::endl;


    std::cerr << "<Sub64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=0;j<=64;++j)
        {
            uint64_t lhs = ran.to<uint64_t>(i);
            uint64_t rhs = ran.to<uint64_t>(j);
            if(lhs<rhs) Swap(lhs,rhs);
            assert(lhs>=rhs);
            const uint64_t dif = lhs-rhs;
            apn L = lhs;
            apn R = rhs;
            apn D = L-R;    Y_ASSERT(D==dif);
            D     = lhs-R;  Y_ASSERT(D==dif);
            D     = L-rhs;  Y_ASSERT(D==dif);
            D = L; D -= R;  Y_ASSERT(D==dif);
            D = L; D -= rhs;Y_ASSERT(D==dif);
        }

    }

    for(apn i=10;i>0;--i) std::cerr << i << "/";
    std::cerr << std::endl;
    for(apn i=10;i>0;i--) std::cerr << i << "/";
    std::cerr << std::endl;

    std::cerr << "<Mul64>" << std::endl;
    for(unsigned i=0;i<=32;++i)
    {
        for(unsigned j=0;j<=32;++j)
        {
            const uint64_t lhs = ran.to<uint64_t>(i);
            const uint64_t rhs = ran.to<uint64_t>(j);
            const uint64_t prod = rhs*lhs;
            {
                apn L = lhs;
                apn R = rhs;
                apn P = L*R;     Y_ASSERT(P==prod);
                P = lhs*R;       Y_ASSERT(P==prod);
                P = L*rhs;       Y_ASSERT(P==prod);
                P = L; P *= R;   Y_ASSERT(P==prod);
                P = L; P *= rhs; Y_ASSERT(P==prod);

            }

        }
    }

    std::cerr << "<Div64>" << std::endl;
    for(unsigned i=0;i<=64;++i)
    {
        for(unsigned j=1;j<=64;++j)
        {
            const uint64_t num = ran.to<uint64_t>(i);
            const uint64_t den = ran.to<uint64_t>(j);
            const uint64_t q   = num/den;
            const uint64_t r   = num%den;
            {
                const apn N = num;
                const apn D = den;
                { const apn Q = N/D;   Y_ASSERT(Q==q); }
                { const apn Q = N/den; Y_ASSERT(Q==q); }
                { const apn Q = num/D; Y_ASSERT(Q==q); }
                { apn Q=N; Q/=D;       Y_ASSERT(Q==q); }
                { apn Q=N; Q/=den;     Y_ASSERT(Q==q); }
                { const apn R = N%D;   Y_ASSERT(R==r); }
                { const apn R = N%den; Y_ASSERT(R==r); }
                { const apn R = num%D; Y_ASSERT(R==r); }
                { apn R=N; R%=D;       Y_ASSERT(R==r); }
                { apn R=N; R%=den;     Y_ASSERT(R==r); }

            }

        }
    }


    std::cerr << "<Cast>" << std::endl;

    {
        for(unsigned i=0;i<=7;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint8_t u8 = 0;
            int8_t  i8 = 0;
            Y_ASSERT(U.tryCast(u8)); Y_ASSERT(u==u8);
            Y_ASSERT(U.tryCast(i8)); Y_ASSERT(u==uint8_t(i8));
        }

        for(unsigned i=0;i<=8;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint8_t u8 = 0;
            Y_ASSERT(U.tryCast(u8)); Y_ASSERT(u==u8);
        }

    }

    {
        for(unsigned i=0;i<=15;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint16_t u16 = 0;
            int16_t  i16 = 0;
            Y_ASSERT(U.tryCast(u16)); Y_ASSERT(u==u16);
            Y_ASSERT(U.tryCast(i16)); Y_ASSERT(u==uint16_t(i16));
        }

        for(unsigned i=0;i<=16;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint16_t u16 = 0;
            Y_ASSERT(U.tryCast(u16)); Y_ASSERT(u==u16);
        }

    }


    {
        for(unsigned i=0;i<=31;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint32_t u32 = 0;
            int32_t  i32 = 0;
            Y_ASSERT(U.tryCast(u32)); Y_ASSERT(u==u32);
            Y_ASSERT(U.tryCast(i32)); Y_ASSERT(u==uint32_t(i32));
        }

        for(unsigned i=0;i<=32;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint32_t u32 = 0;
            Y_ASSERT(U.tryCast(u32)); Y_ASSERT(u==u32);
        }

    }


    {
        for(unsigned i=0;i<=63;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint64_t u64 = 0;
            int64_t  i64 = 0;
            Y_ASSERT(U.tryCast(u64)); Y_ASSERT(u==u64);
            Y_ASSERT(U.tryCast(i64)); Y_ASSERT(u==uint64_t(i64));
        }

        for(unsigned i=0;i<=64;++i)
        {
            const uint64_t u = ran.to<uint64_t>(i);
            const apn      U = u;
            uint64_t u64 = 0;
            Y_ASSERT(U.tryCast(u64)); Y_ASSERT(u==u64);
        }

    }


    {
        apn n(100,ran);
        std::cerr << "n=" << n << std::endl;
        n.ldz();
        std::cerr << "n=" << n << std::endl;
    }
    {
        apn n(100,ran);
        std::cerr << "n=" << n << std::endl;
        n.ld1();
        std::cerr << "n=" << n << std::endl;
    }


#define Y_CHK8(x)    \
std::cerr << "\t\t-- using " << x << " --" << std::endl;\
n = x;               \
Y_CHECK(n._eq(x));   \
Y_CHECK(!n._lt(x));  \
Y_CHECK(!n._gt(x));  \
Y_CHECK(!n._not(x)); \
Y_CHECK(n._geq(x));  \
Y_CHECK(n._leq(x))


    std::cerr << "Cmp8" << std::endl;
    {
        apn n;

        Y_CHK8(0); Y_CHECK(n._lt(1)); Y_CHECK(n._leq(1));
        Y_CHK8(1);


    }


}
Y_UDONE()
