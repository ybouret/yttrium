Y_CreateTest(lingo)

Y_LinkLibraries(test-lingo y-lingo)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(source  test-lingo source)
add_test(chardb  test-lingo chardb)
add_test(makerx  test-lingo makerx)
add_test(nbits   test-lingo nbits)
add_test(pattern test-lingo pattern)
add_test(dict    test-lingo dict)
add_test(dict    test-lingo posix)
add_test(dict    test-lingo rx)


