#include "y/utest/driver.hpp"

Y_UTEST_DECL(32)
{
    Y_UTEST(zero_flux);
    Y_UTEST(bitmap);
    Y_UTEST(tess);

    Y_UTEST(options);
    Y_UTEST(bmp);
    Y_UTEST(jpeg);
    Y_UTEST(png);
    Y_UTEST(tiff);
    Y_UTEST(pnm);
    Y_UTEST(image);
    Y_UTEST(hist);
    Y_UTEST(blur);
    Y_UTEST(filter);
    Y_UTEST(grad);
    Y_UTEST(block);
    Y_UTEST(make5);
    Y_UTEST(makef);

    Y_UTEST(dilate);
    Y_UTEST(erode);
    Y_UTEST(median);
    Y_UTEST(mean);
    Y_UTEST(chain);
    Y_UTEST(edges);
}
Y_UTEST_EXEC()

