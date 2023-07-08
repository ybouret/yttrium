
#include "y/hashing/des64.hpp"
#include "y/hashing/ibj64.hpp"

#include "y/utest/run.hpp"

#include <ctime>

using namespace Yttrium;




Y_UTEST(hashing_mix64)
{

    Hashing::DES64 des64;
    Hashing::IBJ64 ibj64;

    Y_ASSERT(des64.test());

}
Y_UDONE()

