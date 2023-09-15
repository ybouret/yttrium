
#include "y/associative/little-endian-address.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(associative_lek)
{
    { uint8_t  a = 0xab;   LittleEndianKey k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<1>());}
    { uint16_t a = 0x1234; LittleEndianKey k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<2>());}
    { uint32_t a = 0xabcd; LittleEndianKey k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<4>());}
    { uint64_t a = 0x0123; LittleEndianKey k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<8>());}

    {
        int a = 0;
        void *p = &a;
        LittleEndianKey k(p);
        std::cerr << "k=" << k << std::endl;
        LittleEndianAddress K(a);
        std::cerr << "K=" << k << std::endl;
        std::cerr << "p@" << p << ", K@" << K.fetch() << std::endl;
    }

}
Y_UDONE()
