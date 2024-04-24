#include "y/utest/driver.hpp"
#include "y/config/compiler.h"

Y_UTEST_DECL(32)
{
    Y_UTEST(species);
    Y_UTEST(components);
    Y_UTEST(eqs);
    Y_UTEST(weasel);
    Y_UTEST(warden);
}
Y_UTEST_EXEC()

