Y_CreateTest(ink
format)

Y_LinkLibraries(test-ink y-ink)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()
add_test(zero_flux test-ink zero_flux)
add_test(bitmap test-ink bitmap)
add_test(tess   test-ink tess)
add_test(format:options test-ink format_options)
add_test(format:bmp     test-ink format_bmp)
