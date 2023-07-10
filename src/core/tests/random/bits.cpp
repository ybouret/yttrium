#include "y/random/park-miller.hpp"
#include "y/system/seed.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/type/utils.hpp"

#include "y/utest/run.hpp"

#include <cmath>

using namespace Yttrium;

template <typename T>
static inline void TestBits( Random::Bits &ran )
{
    std::cerr << std::endl;

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


    Y_SIZEOF(Random::Bits);

    Random::Rand       ran1;
    Random::ParkMiller ran2( SystemSeed::Get() );

    for(size_t i=0;i<10;++i)
    {
        std::cerr << ran1.to<double>() << std::endl;
    }


    for(size_t i=0;i<10;++i)
    {
        std::cerr << ran2.to<double>() << std::endl;
    }

    for(size_t i=0;i<10;++i)
    {
        std::cerr << "seed=" << SystemSeed::Get() << std::endl;
    }

    TestBits<double>(ran1);
    TestBits<float>(ran1);

    TestBits<double>(ran2);
    TestBits<float>(ran2);


}
Y_UDONE()

