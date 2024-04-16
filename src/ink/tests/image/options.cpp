#include "y/ink/image/format/options.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(format_options)
{
    Ink::FormatOptions opts;

    for(int i=1;i<argc;++i)
    {
        opts << argv[i];
    }

    std::cerr << "opts=" << opts << std::endl;
    std::cerr << "opt[z]='" << opts['z'] << "'" << std::endl;
}
Y_UDONE()

