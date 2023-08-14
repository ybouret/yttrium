#include "y/mkl/api.hpp"
#include "y/utest/run.hpp"
#include "../main.hpp"

using namespace Yttrium;


template <typename T>
static inline void TestFabs(Random::Bits &ran)
{
    
}

Y_UTEST(mkl_api)
{
    Random::Rand ran;

    TestFabs<float>(ran);

}
Y_UDONE()

