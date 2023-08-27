Y_CreateTest(mkl
ode)

Y_LinkLibraries(test-mkl y-mkl)

enable_testing()

add_test(triplet test-mkl triplet)
add_test(ode:rk4 test-mkl ode_rk4)
