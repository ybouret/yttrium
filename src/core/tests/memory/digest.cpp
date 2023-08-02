#include "y/memory/digest.hpp"
#include "y/utest/run.hpp"

using namespace  Yttrium;

Y_UTEST(memory_digest)
{

    {
        Digest d(6);
        std::cerr << d << std::endl;
    }

    {
        Digest d = "";
        std::cerr << d << std::endl;
    }

    {
        Digest d = "a";
        std::cerr << d << std::endl;
    }

    {
        Digest d = "ab";
        std::cerr << d << std::endl;
    }

    {
        Digest d = "abc";
        std::cerr << d << std::endl;
    }

}
Y_UDONE()
