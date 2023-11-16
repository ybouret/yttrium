#include "y/calculus/base10.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/text/plural.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


namespace
{
    static inline void testBase10(const uint64_t u)
    {
        const unsigned n = Base10::DigitsFor(u);
        std::cerr << u << " -> " << n << " digit" << Plural::s(n) << std::endl;
        const Base10::Format fmt = u;
        std::cerr << " |_fmt : '" << *fmt << "'" << std::endl;
    }

}

Y_UTEST(calculus_base10)
{
    for(int i=1;i<argc;++i)
    {
        const uint64_t u = ASCII::Convert::To<uint64_t>(argv[i],"uint64");
        testBase10(u);
    }

    {
        const uint64_t u = UnsignedInt<8>::Maximum;
        testBase10(u);
    }
}
Y_UDONE()

