#include "y/type/div.hpp"
#include "y/utest/run.hpp"
#include "y/memory/out-of-reach.hpp"


using namespace Yttrium;

namespace
{
    template <typename T>
    static inline
    void testDiv(const char *className)
    {
        std::cerr << std::setw(8) << className << " => ";
        typedef typename Div<T>::API DivAPI;
        std::cerr << DivAPI::CallSign << std::endl;
        const T numer = 254;
        const T denom = 13;
        const typename Div<T>::Type dv = DivAPI::Call(numer,denom);
        std::cerr << numer << "/" << denom << " = " << dv.quot << " + " << dv.rem << "/" << denom << std::endl;
        Y_CHECK(dv.quot*denom+dv.rem==numer);
    }
}

#define Y_TEST_DIV(CLASS) testDiv<CLASS>( #CLASS )

Y_UTEST(type_div)
{
    Y_SIZEOF(int);
    Y_SIZEOF(long);
    Y_SIZEOF(long long);

    Y_TEST_DIV(int8_t);
    Y_TEST_DIV(int16_t);
    Y_TEST_DIV(int32_t);
    Y_TEST_DIV(int64_t);


}
Y_UDONE()
