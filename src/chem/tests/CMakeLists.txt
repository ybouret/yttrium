Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-mkl y-jive)

enable_testing()

add_test(algebraic test-chem algebraic)
add_test(simple    test-chem simple)
add_test(lib       test-chem lib)
add_test(lang      test-chem lang)
