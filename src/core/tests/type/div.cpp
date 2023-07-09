#include "y/type/div.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace
{
    template <typename T>
    static inline
    void testDiv(const char *className)
    {
        std::cerr << std::setw(8) << className << " => ";
        typedef typename Div<T>::DivType DivType;
        std::cerr << DivType::CallSign << std::endl;
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
