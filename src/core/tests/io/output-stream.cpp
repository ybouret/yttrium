
#include "y/io/libc/output.hpp"
#include "y/io/data/output.hpp"

#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace Yttrium;

Y_UTEST(io_ostream)
{
    { Libc::OutputFile fp( StdOut ); }
    { Libc::OutputFile fp( StdErr ); }

    { Libc::OutputFile fp( Y_STDOUT ); }
    { Libc::OutputFile fp( Y_STDOUT ); }

    { Libc::OutputFile fp( StdOut ); }
    { Libc::OutputFile fp( StdErr ); }

    {
        Libc::OutputFile fp( StdOut );
        fp << "Hello, World!\n";

        const String s = "Hehe!";
        fp << s << "\n";

    }

    {
        Libc::OutputFile fp("out.dat");
        fp << "Some data...\n";
    }

    {
        Libc::OutputFile fp("out.dat",true);
        fp << "Some more data...\n";
    }

    {
        Libc::OutputFile fp("out.dat",true);
        fp << "Some more data...\n";
    }

    {
        Libc::OutputFile fp("out.dat",true);
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

#if 0
    {
        OutputDataStream< Vector<char> > fp;
        fp("Pi=%g",3.1431);
        std::cerr << fp << std::endl;
    }

    {
        OutputDataStream< List<char> > fp;
        fp("Pi=%g",3.1431);
        std::cerr << fp << std::endl;
    }
#endif

}
Y_UDONE()
