
#include "y/chemical/library.hpp"
#include "y/stream/libc/input.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(lib)
{

    Library        lib;
    const Species &h = lib("H^+");
    const Species &w = lib("HO^-");

    std::cerr << "h='" << h << "'" << std::endl;
    std::cerr << "w='" << w << "'" << std::endl;

    std::cerr << "lib=" << lib << std::endl;

    lib.toBinary("lib.dat");
    {
        Library reloaded;
        InputFile fp("lib.dat");
        reloaded.readFrom(fp);
        std::cerr << "cpy=" << reloaded << std::endl;
    }




}
Y_UDONE()
