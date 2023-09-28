#include "y/rtld/dll.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

Y_UTEST(rtld_dll)
{

    if(argc>1)
    {
        DLL dll(argv[1]);
    }
}
Y_UDONE()

