
#include "y/associative/little-endian-address.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(associative_lek)
{
    { uint8_t  a = 0xab;   Little_Endian_Key k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<1>());}
    { uint16_t a = 0x1234; Little_Endian_Key k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<2>());}
    { uint32_t a = 0xabcd; Little_Endian_Key k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<4>());}
    { uint64_t a = 0x0123; Little_Endian_Key k(a); std::cerr << "k=" << k << std::endl; Y_CHECK(a==k.get<8>());}

    {
        int a = 0;
        void *p = &a;
        Little_Endian_Key k(p);
        std::cerr << "k=" << k << std::endl;
        Little_Endian_Address K(a);
        std::cerr << "K=" << k << std::endl;
        std::cerr << "p@" << p << ", K@" << K.fetch() << std::endl;
    }

}
Y_UDONE()
