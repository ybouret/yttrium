



#include "y/random/bits.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


template <typename T>
static inline void find(const char *name)
{
    std::cerr << "-------- " << name << std::endl;
    static const uint64_t one = 1;
    unsigned     shift        = 32;
    uint64_t     top          = one << shift;

    while(true)
    {
        const T maxDen(top);
        const T maxNum = T(top-1)+T(0.5);
        if(maxNum/maxDen<T(1))
        {
            std::cerr << "maxOk@2^" << shift << std::endl;
            break;
        }
        --shift;
        top >>= 1;
    }

    while(true)
    {
        const T maxDen(top);
        const T maxNum = T(0.5);
        if(maxNum/maxDen>0)
        {
            std::cerr << "minOk@2^" << shift << std::endl;
            break;
        }
        --shift;
        top >>= 1;
    }



}

#include <cstdlib>

Y_UTEST(random_bits)
{

    //find<double>("double");
    //find<long double>("long double");
    //find<float>("float");

    Random::Bits ran(RAND_MAX);

}
Y_UDONE()

