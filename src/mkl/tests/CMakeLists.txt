Y_CreateTest(mkl
ode
opt
lak)

Y_LinkLibraries(test-mkl y-mkl)

enable_testing()

add_test(triplet test-mkl triplet)

add_test(ode:rk4 test-mkl ode_rk4)

add_test(opt:parabolic test-mkl opt_parabolic)
add_test(opt:bracket   test-mkl opt_bracket)

add_test(lak:lu        test-mkl lak_lu)
add_test(lak:ortho_space test-mkl lak_ortho_space)


