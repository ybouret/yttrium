Y_CreateTest(chemical)

Y_LinkLibraries(test-chemical y-chemical y-jive y-mkl y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(init test-chemical init)
add_test(weasel test-chemical weasel)
add_test(plexus test-chemical plexus)

