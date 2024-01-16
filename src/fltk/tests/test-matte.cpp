#include "y/utest/run.hpp"
#include "matte.h"

using namespace Yttrium;

Y_UTEST(matte)
{
    MakeWindowMatte()->show(argc,argv);
    
    (void)Fl::run();
}
Y_UDONE()
