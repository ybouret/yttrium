#include "y/kemp/natural.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_UTEST(kemp_natural)
{
    Random::ParkMiller ran;
    for(unsigned i=0;i<=64;++i)
    {
        const uint64_t lhs = ran.to<uint64_t>(i);
        const apn      L(lhs);
        std::cerr << Hexadecimal(lhs,Hexadecimal::Compact) << "/" << L << std::endl;
    }

    for(unsigned i=0;i<=80;++i)
    {
        const apn n(Kemp::TwoToThe,i);
        std::cerr << n << std::endl;
    }

    std::cerr << "<Add/Cmp64>" << std::endl;
    for(unsigned i=0;i<=31;++i)
    {
        for(unsigned j=0;j<=31;++j)
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


}
Y_UDONE()
