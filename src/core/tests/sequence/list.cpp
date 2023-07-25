
#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(sequence_list)
{
    List<int> l;

    l << 2;
    l >> 3;

    std::cerr << l << std::endl;

}
Y_UDONE()


