

#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(32)
{
    Y_UTEST(algebraic);
    Y_UTEST(simple);
    Y_UTEST(lib);
    Y_UTEST(lang);
    Y_UTEST(weasel);
    Y_UTEST(plexus);
    Y_UTEST(guardian);
}
Y_UTEST_EXEC()

