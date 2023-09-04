#include "y/ordered/vector.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(ordered_vector)
{
    { OrderedVector<int> V0; }

    OrderedVector<int> V(5);

    V.search(2);



}
Y_UDONE()

