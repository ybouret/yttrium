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
        std::cerr << std::endl;
        std::cerr << std::setw(8) << className << " => ";
        typedef typename Div<T>::API DivAPI;
        std::cerr << DivAPI::CallSign << std::endl;
        const T numer = 100;
        const T denom = 13;
        const typename Div<T>::Type dv = DivAPI::Call(numer,denom);
        std::cerr << int64_t(numer) << "/" << int64_t(denom) << " = " << int64_t(dv.quot) << " + " << int64_t(dv.rem) << "/" << int64_t(denom) << std::endl;
        Y_CHECK(dv.quot*denom+dv.rem==numer);
        std::cerr << std::endl;
    }
}

#define Y_TEST_DIV(CLASS) testDiv<CLASS>( #CLASS )

Y_UTEST(type_div)
{
    Y_SIZEOF(int);
    Y_CHECK(0!=Div<int>::Call);

    Y_SIZEOF(long);
    Y_CHECK(0!=Div<long>::Call);

    Y_SIZEOF(long long);
    Y_CHECK(0!=Div<long long>::Call);



    Y_TEST_DIV(int8_t);
    Y_TEST_DIV(int16_t);
    Y_TEST_DIV(int32_t);
    Y_TEST_DIV(int64_t);

#if 0
    uint64_t qw = 0;
    uint16_t w  = 0xabcd;
    Memory::OutOfReach::Fill(&qw, sizeof(qw), &w, sizeof(w) );
    std::cerr << "qw=" << std::hex << qw << std::endl;
#endif
}
Y_UDONE()
