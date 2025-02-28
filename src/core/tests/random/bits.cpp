#include "y/random/park-miller.hpp"
#include "y/random/mt19937.hpp"
#include "y/random/isaac.hpp"

#include "y/system/seed.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/type/utils.hpp"

#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/utest/timing.hpp"

#include <cmath>

using namespace Yttrium;


template <typename T>
static inline void TestBits( Random::Bits &ran )
{

    static const size_t n = 4096;
    T dbl[n];
    for(size_t i=0;i<n;++i)
    {
        dbl[i] = ran.to<T>();
    }

    HeapSort::Tableau(dbl,n,Comparison::Increasing<double>);


    T sum = 0;
    for(size_t i=0;i<n;++i) sum += dbl[i];
    const T ave = sum/n;
    std::cerr << "ave=" << ave << std::endl;

    sum = 0;
    for(size_t i=0;i<n;++i) sum += Squared(dbl[i]-ave);
    const T sig = std::sqrt(sum/(n-1));
    std::cerr << "sig=" << sig << std::endl;
}


Y_UTEST(random_bits)
{


    for(size_t i=0;i<10;++i)
    {
        std::cerr << "seed=" << SystemSeed::Get() << std::endl;
    }

    Y_SIZEOF(Random::Bits);


    Random::Rand        ran1;
    Random::ParkMiller  ran2;
    Random::MT19937     ran3;
    Random::ISAAC<4>    ran4;
    Random::ISAAC<8>    ran5;

    Random::Bits * const hBits[] = {
        &ran1,
        &ran2,
        &ran3,
        &ran4,
        &ran5
    };

    const size_t nBits = sizeof(hBits) / sizeof(hBits[0]);

    Timing tmx;
    for(size_t i=0;i<nBits;++i)
    {
        Random::Bits &ran = *hBits[i];
        std::cerr << "-------- " << ran.callSign() << std::endl;
        TestBits<double>(ran);
        TestBits<float>(ran);

        Y_Timing(tmx, volatile uint32_t r = ran.next32(); (void)r, 0.5);
        std::cerr << "\tspeed = " << HumanReadable( tmx.speed() ) << std::endl;
        std::cerr << std::endl;
    }







    std::cerr << "Testing Random Bytes" << std::endl;
    for(unsigned lo=0;lo<=255;++lo)
    {
        const double half  = 0.5;
        const double start = lo;
        for(unsigned hi=lo;hi<=255;++hi)
        {
            const double delta = double(hi) - double(lo);
            for(size_t loop=0;loop<1000;++loop)
            {
                const uint8_t r    = static_cast<uint8_t>(floor( start + ran1.to<double>() * delta + half ));
                Y_ASSERT(r>=lo);
                Y_ASSERT(r<=hi);
            }
        }
    }



}
Y_UDONE()

