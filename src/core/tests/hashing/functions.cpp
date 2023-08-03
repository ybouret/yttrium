

#include "y/hashing/sha1.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"
#include "y/associative/suffix/map.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;




#define HREG(NAME) \
do {\
const Hashing::Function::Pointer h =  Hashing::NAME::Create(); \
const String                     k = h->callSign();            \
Y_ASSERT(hmap.insert(k,h));                                     \
} while(false)

Y_UTEST(hashing_functions)
{

    Hashing::MD5::    Tests();
    Hashing::SHA1::   Tests();
    Hashing::SHA256:: Tests();
    Hashing::SHA224:: Tests();

    SuffixMap<String,Hashing::Function::Pointer> hmap;

    HREG(MD5);
    HREG(SHA1);
    HREG(SHA256);
    HREG(SHA224);
    HREG(SHA512);
    HREG(SHA384);

    

}
Y_UDONE()

