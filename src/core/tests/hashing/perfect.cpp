#include "y/hashing/perfect.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

#define HASHOF(KEY) std::cerr << "mph(" << KEY << ")=" << mph(KEY) << std::endl

Y_UTEST(hashing_perfect)
{
    Hashing::Perfect mph;
    const char key1[] = "hello";
    const String key2 = "world";
    const String key3 = "hell";
    const String nope = "nope";

    mph(key1,0);
    mph(key2,1);
    mph(key3,2);

    HASHOF(key1);
    HASHOF(key2);
    HASHOF(key3);

    Y_CHECK(0==mph(key1));
    Y_CHECK(1==mph(key2));
    Y_CHECK(2==mph(key3));
    Y_CHECK(mph(nope)<0);



}
Y_UDONE()

