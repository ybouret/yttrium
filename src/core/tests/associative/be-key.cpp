
#include "y/associative/be-key.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(associative_be_key)
{
    { uint8_t  a = 0xab;   BE_Key k(a); std::cerr << "k=" << k << std::endl; }
    { uint16_t a = 0x1234; BE_Key k(a); std::cerr << "k=" << k << std::endl; }
    { uint32_t a = 0xabcd; BE_Key k(a); std::cerr << "k=" << k << std::endl; }
    { uint64_t a = 0x0123; BE_Key k(a); std::cerr << "k=" << k << std::endl; }

    { int a = 0; void *p = &a; BE_Key k(p); std::cerr << "k=" << k << std::endl; }

}
Y_UDONE()
