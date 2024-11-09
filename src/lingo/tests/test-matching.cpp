
#include "y/lingo/pattern/matching.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;
using namespace Lingo;

Y_UTEST(matching)
{
    if(argc>1)
    {
        Matching matching = argv[1];
        if(argc>2)
        {
            InputFile fp(argv[2]);
            unsigned  iLine = 0;
            String    line;
            while( fp.gets(line) )
            {
                ++iLine;
                const String name = argv[2] + Formatted::Get(":%u",iLine);
                std::cerr << "matching '" << line << "'" << std::endl;

                if( matching.exactly(name,line) )
                {
                    std::cerr << " (+exactly) '" << matching << "'" << std::endl;
                }
                else
                    std::cerr << " (-exactly)" << std::endl;

                if( matching.somehow(name,line) )
                {
                    std::cerr << " (+somehow) '" << matching << "'" << std::endl;
                }
                else
                    std::cerr << " (-somehow)" << std::endl;


            }
        }

    }
}
Y_UDONE()
