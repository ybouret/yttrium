
Y_CreateTest(lua++)

Y_LinkLibraries(test-lua++ y-lua++ y-lua)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
#set( DATA ${HERE}/../../../var)

enable_testing()

add_test(state    test-lua++ state ${HERE}/sample.lua)
add_test(function test-lua++ function)
