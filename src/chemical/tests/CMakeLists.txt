Y_CreateTest(chemical)

Y_LinkLibraries(test-chemical y-chemical y-lingo  y-mkl y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(weasel test-chemical weasel)
add_test(lib    test-chemical lib)
add_test(cm     test-chemical cm)
add_test(eqs    test-chemical eqs)
add_test(plexus test-chemical plexus)
add_test(plexus test-chemical fluo)

