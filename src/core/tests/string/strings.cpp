#include "y/string/string.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(strings)
{
    Y_SIZEOF(Core::String<char>);
    Y_SIZEOF(Core::String<uint32_t>);

    Core::String<char> s;
    Core::String<uint32_t> S;

    std::cerr << "s: " << s.capacity() << std::endl;
    std::cerr << "S: " << S.capacity() << std::endl;



}
Y_UDONE()

