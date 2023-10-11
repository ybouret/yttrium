#include "y/lua++/state.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(state)
{
    Lua::VM vm = new Lua::State();

    if(argc>1)
    {
        vm->dofile(argv[1]);

        const lua_Integer I = vm->get<lua_Integer>("I"); std::cerr << I << std::endl;
        const double      F = vm->get<lua_Number>("F");  std::cerr << F << std::endl;
        const String      S = vm->get<String>("S");      std::cerr << S << std::endl;
    }

}
Y_UDONE()
