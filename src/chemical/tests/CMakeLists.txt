Y_CreateTest(chemical)

Y_LinkLibraries(test-chemical y-chemical y-mkl y-jive y-lua++ y-lua)

enable_testing()
add_test(species test-chemical species)
add_test(rosary  test-chemical rosary)

