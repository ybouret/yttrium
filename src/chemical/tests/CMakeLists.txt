Y_CreateTest(chemical)

Y_LinkLibraries(test-chemical y-chemical y-mkl y-jive y-lua++ y-lua)

enable_testing()
add_test(species    test-chemical species)
add_test(components test-chemical components)
add_test(eqs        test-chemical eqs)
add_test(limits     test-chemical limits)
add_test(weasel     test-chemical weasel)
add_test(warden     test-chemical warden)
add_test(diff       test-chemical diff)

