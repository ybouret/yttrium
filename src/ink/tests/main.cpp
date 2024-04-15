#include "y/utest/driver.hpp"

Y_UTEST_DECL(16)
{
    Y_UTEST(zero_flux);
    Y_UTEST(bitmap);
    Y_UTEST(tess);

    Y_UTEST(format_options);
}
Y_UTEST_EXEC()

