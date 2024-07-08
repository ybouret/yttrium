
#include "y/chemical/weasel/compiler.hpp"
#include "y/utest/run.hpp"

//#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{

    Weasel::Compiler &weasel = Weasel::Compiler::Instance();

    Library        lib;
    LuaEquilibria  eqs;

    if(argc>1)
    {
        weasel(lib,eqs,Jive::Module::OpenFile(argv[1]),true);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


}
Y_UDONE()
