#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;


Y_UTEST(strings)
{
    Y_SIZEOF(Core::String<char>);
    Y_SIZEOF(Core::String<uint32_t>);

    {
        Core::String<char> s;
        Core::String<uint32_t> S;

        std::cerr << "s: " << s.capacity() << std::endl;
        std::cerr << "S: " << S.capacity() << std::endl;
    }

    {
        Core::String<char> s = 'a';
        std::cerr << s << std::endl;
    }

    {
        Core::String<char> s = "Hello";
        std::cerr << s << std::endl;
    }

    {
        Core::String<char> s("Hello",3);
        std::cerr << s << std::endl;
    }

    {
        Core::String<char> s = "Hello";
        s << ", World";
        s << '!';
        s << ' ';
        s << s;
        s >> '+';
        s >> " before ";
        s >> s;
        std::cerr << s << std::endl;
    }

    {
        Core::String<char> a = "Hello ";
        Core::String<char> b = " World";
        Core::String<char> s = a+b;
        std::cerr << s << std::endl;
        Iterating::DisplayAll::Of(s);
    }

    {
        Core::String<char> s = "Hello";
        Y_CHECK("Hello"==s);
        Y_CHECK(s=="Hello");
        Y_CHECK("World"!=s);
        Y_CHECK(s!="World");
    }

    {
        const String ans = Formatted::Get("here: %s @%d", program, __LINE__);
        std::cerr << ans << std::endl;
    }


}
Y_UDONE()

