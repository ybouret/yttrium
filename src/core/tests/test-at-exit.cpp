
#include "y/system/at-exit.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    static int l1 = 10;
    static int l2 = -4;
    static void displayInt(void *args)
    {
        const int data = args ? *(int *)args : 0;
        std::cerr << "-- displayInt(" << data << ")" << std::endl;
    }
}

Y_UTEST(atexit)
{
    AtExit::Register(displayInt,&l1,l1);
    AtExit::Register(displayInt,&l2,l2);

}
Y_UDONE()

