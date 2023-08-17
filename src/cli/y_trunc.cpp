
#include "y/program.hpp"
#include "y/stream/libc/input.hpp"

using namespace Yttrium;

Y_Program()
{
    if(argc<=2) { std::cerr << "usage: " << program << " nmin nmax" << std::endl; return 0; }

    const size_t nmin = atoi(argv[1]);
    const size_t nmax = atoi(argv[2]);

    Libc::InputFile fp( StdIn );
    IO::Chars line;
    while( fp.gets(line) )
    {
        const size_t n = line.size;
        if(n>=nmin && n<=nmax) std::cout << line << std::endl;
    }
}
Y_End()
