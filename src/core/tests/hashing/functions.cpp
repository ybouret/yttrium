

#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;




Y_UTEST(hashing_functions)
{

    Hashing::MD5::    Tests();
    Hashing::SHA1::   Tests();
    Hashing::SHA256:: Tests();
    Hashing::SHA224:: Tests();
    

}
Y_UDONE()

