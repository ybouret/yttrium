#include "y/type/complex.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/random/bits.hpp"
#include "y/memory/out-of-reach.hpp"

using namespace Yttrium;


template <typename T>
static inline T bigReal(Random::Bits &ran)
{
    const T r = T(1e9) * ran.to<T>();
    return ran.choice() ? r : -r;
}

void multAlgo(int a, int b, int c, int d)
{
    int re = a*c - b*d;
    int im = b*c + a*d;
    std::cerr << re << "," << im << std::endl;
}

void gaussAlgo(int a, int b, int c, int d)
{
    int t1 = a*c;
    int t2 = b*d;
    int re = t1 - t2;
    int im = (a + b)*(c + d) - t1 - t2;
    std::cerr << re << "," << im << std::endl;
}


template <typename T>
static inline
void testMul(Random::Bits &ran)
{
    Timing     tmx;
    Complex<T> z1,z2;
    volatile Complex<T> z3;



    uint64_t Multi64 = 0;
    uint64_t Gauss64 = 0;
    do
    {
        z1.re = bigReal<T>(ran);
        z1.im = bigReal<T>(ran);
        z2.re = bigReal<T>(ran);
        z2.im = bigReal<T>(ran);
        for(size_t i=0;i<1000;++i)
        {
            {
                const uint64_t mark = WallTime::Ticks();
                (Complex<T> &)z3 = Complex<T>::MultiAlgo(z1, z2);
                Multi64 += WallTime::Ticks() - mark;
            }

            {
                const uint64_t mark = WallTime::Ticks();
                (Complex<T> &)z3 = Complex<T>::GaussAlgo(z1, z2);
                Gauss64 += WallTime::Ticks() - mark;
            }
            ++tmx.cycle;
        }
    } while( tmx(Gauss64) < 0.5 );

    (void) Memory::OutOfReach::Addr((void*)&z3);

    std::cerr << "Mul" << std::setw(3) << sizeof(T)*8 << "-bits :";
    tmx.ticks = Multi64;
    std::cerr << " Multi @" << HumanReadable(tmx.speed());
    tmx.ticks = Gauss64;
    std::cerr << " Gauss @" << HumanReadable(tmx.speed());
    std::cerr << std::endl;
}


Y_UTEST(type_complex)
{
    Random::Rand ran;

    Complex<float> zf(1,2);
    std::cerr << zf << std::endl;

    multAlgo(1, 2, 3, 4);
    gaussAlgo(1, 2, 3, 4);
    std::cerr << Complex<float>::MultiAlgo( Complex<float>(1,2), Complex<float>(3,4) ) << std::endl;
    std::cerr << Complex<float>::GaussAlgo( Complex<float>(1,2), Complex<float>(3,4) ) << std::endl;


    testMul<float>(ran);
    testMul<double>(ran);
    testMul<long double>(ran);




}
Y_UDONE()

