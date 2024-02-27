
#include "y/stream/libc/output.hpp"
#include "y/stream/data/output.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/text/hexadecimal.hpp"

using namespace Yttrium;

Y_UTEST(io_ostream)
{
    { OutputFile fp( StdOut ); }
    { OutputFile fp( StdErr ); }

    { OutputFile fp( Y_STDOUT ); }
    { OutputFile fp( Y_STDOUT ); }

    { OutputFile fp( StdOut ); }
    { OutputFile fp( StdErr ); }

    {
        OutputFile fp( StdOut );
        fp << "Hello, World!\n";

        const String s = "Hehe!";
        fp << s << "\n";

    }

    {
        OutputFile fp("out.dat");
        fp << "Some data...\n";
    }

    {
        OutputFile fp("out.dat",true);
        fp << "Some more data...\n";
    }

    {
        OutputFile fp("out.dat",true);
        fp << "Some more data...\n";
    }

    {
        OutputFile fp("out.dat",true);
        fp("Pi=%g\n",3.14);
        
    }


    {
        String s;
        {
            OutputDataStream<String> fp(s);

            fp("Pi=%g\n--",3.14);
        }

        std::cerr << s << std::endl;
    }




}
Y_UDONE()
