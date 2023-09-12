#include "y/woven/indices.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(woven_indices)
{
    WOVEn::Indices indx(4);
    indx |= 3;
    indx |= 5;
    indx |= 2;
    std::cerr << indx << std::endl;

}
Y_UDONE()

