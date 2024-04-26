Y_CreateTest(ink
image)

Y_LinkLibraries(test-ink y-ink y-jive y-jpeg)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()
add_test(zero_flux test-ink zero_flux)
add_test(bitmap test-ink bitmap)
add_test(tess   test-ink tess)
add_test(format:options test-ink format_options)
add_test(format:bmp     test-ink format_bmp)
add_test(format:jpeg    test-ink format_jpeg)
add_test(format:png     test-ink format_png)
add_test(image          test-ink image)
