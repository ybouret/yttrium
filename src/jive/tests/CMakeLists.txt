Y_CreateTest(jive)

Y_LinkLibraries(test-jive y-jive)

enable_testing()

add_test(context test-jive context)
add_test(char    test-jive char)
add_test(module  test-jive module)
