



#include "y/random/bits.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(random_bits)
{

    {
        const long double maxNum = 4294967295.5L;
        const long double maxDen = 4294967296.0L;

        std::cerr << "maxDen=" << maxDen << std::endl;
        const long double   upper = maxNum/maxDen;
        const long double   lower = 1.0L/maxDen;
        std::cerr << "lower = " << lower << std::endl;
        Y_ASSERT(upper<1.0L);
        Y_ASSERT(lower>0.0L);
    }

    {
        const double maxNum = 4294967295.5;
        const double maxDen = 4294967296.0;

        std::cerr << "maxDen=" << maxDen << std::endl;
        const double   upper = maxNum/maxDen;
        const double   lower = 1.0/maxDen;
        std::cerr << "lower = " << lower << std::endl;
        Y_ASSERT(upper<1.0);
        Y_ASSERT(lower>0.0);
    }

    {
        //const float    maxNum = 4294967295.5f;
        //const float    maxDen = 4294967296.0f;
        const size_t umax = size_t(1) << 23;
        const float maxNum = float(umax-1)+0.5f;
        const float maxDen = float(umax);


        std::cerr << "maxDen=" << maxDen << std::endl;
        const float   upper = maxNum/maxDen;
        const float   lower = 1.0f/maxDen;
        std::cerr << "upper = " << upper << std::endl;
        Y_ASSERT(upper<1.0f);
        Y_ASSERT(lower>0.0);

    }


}
Y_UDONE()

