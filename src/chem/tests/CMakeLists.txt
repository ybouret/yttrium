Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-mkl y-jive y-lua++ y-lua)

enable_testing()

add_test(algebraic test-chem algebraic)
add_test(simple    test-chem simple)
add_test(lib       test-chem lib)
add_test(lang      test-chem lang)
add_test(weasel    test-chem weasel)
add_test(plexus    test-chem plexus)
add_test(custodian test-chem custodian)
add_test(equalizer test-chem equalizer)
