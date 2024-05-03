#include "y/utest/driver.hpp"

Y_UTEST_DECL(16)
{
    Y_UTEST(zero_flux);
    Y_UTEST(bitmap);
    Y_UTEST(tess);

    Y_UTEST(format_options);
    Y_UTEST(format_bmp);
    Y_UTEST(format_jpeg);
    Y_UTEST(format_png);
    Y_UTEST(format_tiff);
    Y_UTEST(image);
    Y_UTEST(hist);
    Y_UTEST(blur);
    Y_UTEST(filter);
}
Y_UTEST_EXEC()

