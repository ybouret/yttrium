#include "y/utest/run.hpp"

#include "y/random/isaac/randtest.c"


Y_UTEST(random_isaac)
{
    (void) isaac_call();
}
Y_UDONE()

