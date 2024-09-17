#include "y/stream/proc/input.hpp"
#include "y/program.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"

using namespace Yttrium;

Y_Program()
{
    if(argc<=1)
    {
        std::cerr << "usage: " << program << " portname" << std::endl;
        return 0;
    }

    const String   port = argv[1];
    Vector<String> fields(32,AsCapacity);;
    {
        const String cmd = "port deps " + port;
        ProcInput    fp(cmd);
        String line;
        while( fp.gets(line) )
        {
            fields.free();
            if(line.size()<=0) continue;
            //Tokenizer::AppendTo(fields, line, ':');
            
        }
    }


}
Y_End()

