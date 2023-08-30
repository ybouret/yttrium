#include "y/system/rtti.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(rtti)
{
    const RTTI &ti = RTTI::Of<int>();
    std::cerr << ti << std::endl;
    std::cerr << RTTI::Name<int>() << std::endl;

    RTTI::Alias<int>("int");
    std::cerr << ti << std::endl;
    std::cerr << RTTI::Name<int>() << std::endl;

    std::cerr << RTTI::Name<char>() << std::endl;
    std::cerr << RTTI::Name<signed char>() << std::endl;
    std::cerr << RTTI::Name<unsigned char>() << std::endl;

    RTTI::Alias<char>("char");
    RTTI::Alias<signed char>("signed char");
    RTTI::Alias<unsigned char>("unsigned char");
    RTTI::Alias<uint8_t>("uint8_t");
    RTTI::Alias<int8_t>("int8_t");

    std::cerr << RTTI::Of<char>() << std::endl;
    std::cerr << RTTI::Of<signed char>() << std::endl;
    std::cerr << RTTI::Of<unsigned char>() << std::endl;
    std::cerr << RTTI::Of<uint8_t>() << std::endl;
    std::cerr << RTTI::Of<int8_t>()  << std::endl;
    std::cerr << RTTI::Of<String>()  << std::endl;


}
Y_UDONE()



