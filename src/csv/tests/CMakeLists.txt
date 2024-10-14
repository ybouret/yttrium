Y_CreateTest(csv)

Y_LinkLibraries(test-csv y-csv y-jive)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(parser test-csv parser)


