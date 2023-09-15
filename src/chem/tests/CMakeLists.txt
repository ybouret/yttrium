Y_CreateTest(chem)

Y_LinkLibraries(test-chem y-chem y-mkl)

enable_testing()

add_test(algebraic test-chem algebraic)
