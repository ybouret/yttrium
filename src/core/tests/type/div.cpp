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
        static const Div<T> divAPI;
        std::cerr << "-- " << std::setw(8) << className << " => ";
        std::cerr << "using " << Div<T>::CallSign << std::endl;

        const T numer = 100;
        const T denom = 13;
        const typename Div<T>::Type dv = divAPI.call(numer,denom);
        std::cerr << int64_t(numer) << "/" << int64_t(denom) << " = " << int64_t(dv.quot) << " + " << int64_t(dv.rem) << "/" << int64_t(denom) << std::endl;
        Y_CHECK(dv.quot*denom+dv.rem==numer);
        std::cerr << std::endl;
    }
}

#define Y_TEST_DIV(CLASS) testDiv<CLASS>( #CLASS )

Y_UTEST(type_div)
{
    Y_SIZEOF(int);       const Div<int>       divInt;      Y_CHECK(0!=divInt.call);      std::cerr << std::endl;
    Y_SIZEOF(long);      const Div<long>      divLong;     Y_CHECK(0!=divLong.call);     std::cerr << std::endl;
    Y_SIZEOF(long long); const Div<long long> divLongLong; Y_CHECK(0!=divLongLong.call); std::cerr << std::endl;



    Y_TEST_DIV(int8_t);
    Y_TEST_DIV(int16_t);
    Y_TEST_DIV(int32_t);
    Y_TEST_DIV(int64_t);

    uint64_t qw = 0;
    uint16_t w  = 0xabcd;
    Memory::OutOfReach::Fill(&qw, sizeof(qw), &w, sizeof(w) );
    std::cerr << "qw=" << std::hex << qw << std::endl;
}
Y_UDONE()
