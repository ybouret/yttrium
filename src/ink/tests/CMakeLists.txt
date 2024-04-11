Y_CreateTest(ink)

Y_LinkLibraries(test-ink y-ink)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()
add_test(zero_flux test-ink zero_flux)
add_test(bitmap test-ink bitmap)
add_test(tess   test-ink tess)
