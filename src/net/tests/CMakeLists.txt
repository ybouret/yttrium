Y_CreateTest(net)

Y_LinkLibraries(test-net y-net)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(init test-net init)
