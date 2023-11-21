#include "y/calculus/gray.hpp"
#include "y/type/ints.hpp"
#include "y/utest/run.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/random/bits.hpp"

using namespace Yttrium;

template <typename T>
static inline
void testGray(Random::Bits &ran)
{
    static const unsigned bits = sizeof(T) * 8;
    std::cerr << "Testing for " << bits << " bits" << std::endl;
    if(bits<32)
    {
        for(uint64_t i=0;i< IntegerFor<T>::Maximum;++i)
        {
            const T n   = static_cast<T>(i);
            const T enc = Gray::Encode(n);
            const T dec = Gray::Decode(enc);
            //std::cerr << Hexadecimal(n) << " => " << Hexadecimal(enc) << " => " << Hexadecimal(dec) << std::endl;
            Y_ASSERT(dec==n);
        }
    }
    else
    {
        for(uint64_t i=0;i< IntegerFor<T>::Maximum;i += 1+ran.leq(127))
        {
            const T n   = static_cast<T>(i);
            const T enc = Gray::Encode(n);
            const T dec = Gray::Decode(enc);
            //std::cerr << Hexadecimal(n) << " => " << Hexadecimal(enc) << " => " << Hexadecimal(dec) << std::endl;
            Y_ASSERT(dec==n);
        }
    }
}

Y_UTEST(calculus_gray)
{
    Random::Rand ran;
    testGray<uint8_t>(ran);
    testGray<uint16_t>(ran);
    testGray<uint32_t>(ran);

}
Y_UDONE()

