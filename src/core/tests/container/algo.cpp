
#include "y/container/algo/no-repeat.hpp"
#include "y/container/algo/crop.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"

using namespace Yttrium;

static inline bool coma(const char c) noexcept { return ',' == c; }

Y_UTEST(container_algo)
{
    String s = "  Hello,,,,World  ";
    
    std::cerr << " => '" << Algo::Crop(s,isspace) << "'" << std::endl;
    std::cerr << " => '" << Algo::NoRepeat(s,coma) << "'" << std::endl;

}
Y_UDONE()
