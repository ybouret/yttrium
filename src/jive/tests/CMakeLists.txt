Y_CreateTest(jive)

Y_LinkLibraries(test-jive y-jive)

enable_testing()

add_test(context test-jive context)
add_test(char    test-jive char)
add_test(module  test-jive module)
add_test(fc      test-jive fc)
add_test(pattern test-jive pattern)
add_test(posix   test-jive posix)
add_test(dict    test-jive dict)
add_test(rx      test-jive rx)
add_test(scanner test-jive scanner)
add_test(match   test-jive match)
add_test(lexer   test-jive lexer)

