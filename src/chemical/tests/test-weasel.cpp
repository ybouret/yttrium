
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

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


}
Y_UDONE()
