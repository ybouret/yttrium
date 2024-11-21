Y_CreateTest(lingo)

Y_LinkLibraries(test-lingo y-lingo)

set( HERE ${CMAKE_CURRENT_SOURCE_DIR} )
set( DATA ${HERE}/../../../var)

enable_testing()

add_test(source   test-lingo source)
add_test(chardb   test-lingo chardb)
add_test(makerx   test-lingo makerx)
add_test(nbits    test-lingo nbits)
add_test(pattern  test-lingo pattern)
add_test(dict     test-lingo dict)
add_test(posix    test-lingo posix)
add_test(rx       test-lingo rx)
add_test(matching test-lingo matching)
add_test(scanner  test-lingo scanner)
add_test(lexer    test-lingo lexer)
add_test(syntax   test-lingo syntax)
add_test(grammar  test-lingo grammar)
add_test(parser   test-lingo parser ${DATA}/db.js)
add_test(parser1   test-lingo parser ${DATA}/sample1.js)
add_test(parser2   test-lingo parser ${DATA}/sample2.js)
add_test(parser3   test-lingo parser ${DATA}/sample3.js)
add_test(parser4   test-lingo parser ${DATA}/sample4.js)
add_test(parser-nl test-lingo parser_nl)
add_test(types     test-lingo types)
add_test(eval      test-lingo eval)


