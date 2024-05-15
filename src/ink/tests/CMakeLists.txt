Y_CreateTest(ink
image)

#link_directories(${CMAKE_CURRENT_BIN_DIR})
Y_LinkLibraries(test-ink y-ink y-jive y-mkl y-jpeg png_static y-z tiff)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()
add_test(zero_flux test-ink zero_flux)
add_test(bitmap    test-ink bitmap)
add_test(tess      test-ink tess)
add_test(options   test-ink options)
add_test(bmp       test-ink bmp)
add_test(jpeg      test-ink jpeg)
add_test(png       test-ink png)
add_test(tiff      test-ink tiff)
add_test(pnm       test-ink pnm)
add_test(image     test-ink image)
add_test(hist      test-ink hist)
add_test(blur      test-ink blur)
add_test(filter    test-ink filter)
add_test(grad      test-ink grad)
add_test(block     test-ink block)
add_test(makef     test-ink makef)
add_test(dilate    test-ink dilate)
add_test(erode     test-ink erode)
add_test(median    test-ink median)
add_test(mean      test-ink mean)



