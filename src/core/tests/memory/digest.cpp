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

    if(argc>1)
    {
        Digest d = argv[1];
        std::cerr << argv[1] << " -> " << d << std::endl;
    }

}
Y_UDONE()
