
#include "y/io/libc/output.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"

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


}
Y_UDONE()
