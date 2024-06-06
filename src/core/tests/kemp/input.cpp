#include "y/kemp/rational.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(kemp_input)
{
    for(int i=1;i<argc;)
    {
        const String opt = argv[i++];
        if(opt=='n')
        {
            if(i<argc)
            {
                const String s = argv[i++];
                const apn    n = s;
                std::cerr << s << " => " << n.toDec() << " / 0x" << n.toHex() << std::endl;
            }
            continue;
        }


        if(opt=='i')
        {
            if(i<argc)
            {
                const String s = argv[i++];
                const apz    z = s;
                std::cerr << s << " => " << z << std::endl;
            }
            continue;
        }
    }

    return 0;

    String s = "1234";
    if(argc>1)
    {
        s = argv[1];
    }
    apn n = s;
    std::cerr << s << " => " << n.toDec() << " / 0x" << n.toHex() << std::endl;
}
Y_UDONE()

