Y_CreateTest(json)

Y_LinkLibraries(test-json y-json y-jive y-lingo)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(json:db test-json   ${DATA}/db.js)
add_test(json:1  test-json   ${DATA}/sample1.js)
add_test(json:2  test-json   ${DATA}/sample2.js)
add_test(json:3  test-json   ${DATA}/sample3.js)
add_test(json:4  test-json   ${DATA}/sample4.js)
