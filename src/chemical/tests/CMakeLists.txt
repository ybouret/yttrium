Y_CreateTest(chemical)

Y_LinkLibraries(test-chemical y-chemical)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(init test-chemical init)
