#include "y/jive/pattern/matcher.hpp"
#include "y/utest/run.hpp"
#include "y/stream/libc/input.hpp"
#include "y/jive/source.hpp"

using namespace Yttrium;

Y_UTEST(match)
{
    if(argc>1)
    {
        Jive::Matcher match = argv[1];
        std::cerr << "-- fragile  : " << match.motif->isFragile() << std::endl;
        std::cerr << "-- univocal : " << match.motif->isUnivocal() << std::endl;

        if(argc>2)
        {
            const String    fileName = argv[2];
            Libc::InputFile fp(fileName);
            IO::Chars       line;
            unsigned        iline = 1;
            while( fp.gets(line) )
            {
                const String uuid = fileName + Formatted::Get(":%u",iline);
                std::cerr << "-- '" << line << "'" << std::endl;
                {

                    {
                        Jive::Token *token = match.exactly(uuid,line);
                        if(token)
                        {
                            std::cerr << "-- exactly: " << *token << std::endl;
                        }
                    }

                    {
                        Jive::Token *token = match.somehow(uuid,line);
                        if(token)
                        {
                            std::cerr << "-- somehow: " << *token << std::endl;
                        }
                    }

                    

                }

            }
        }


    }

}
Y_UDONE()
