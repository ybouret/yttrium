#include "y/cipher/stream/arc4.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

using namespace Yttrium;

Y_UTEST(cipher_arc4)
{
    const String key = "Hello";
    Crypto::ARC4 sc(key);


}
Y_UDONE()

